#include <chrono>
#include <cassert>
#include <iostream>
#include <random>
#include <functional>
#include <limits>
#include <omp.h>
#include "Machine.hpp"
#include "instruction/Instruction.hpp"

Machine genMachine(std::mt19937& random) {
	const static std::array<std::function<std::unique_ptr<Instruction>(int)>, 9> constructors{
			[](int rand) { return std::make_unique<Immediate>(rand); },
			[](int rand) { return std::make_unique<Add>(); },
			[](int rand) { return std::make_unique<Sub>(); },
			[](int rand) { return std::make_unique<Mul>(); },
			[](int rand) { return std::make_unique<Div>(); },
			[](int rand) { return std::make_unique<Not>(); },
			[](int rand) { return std::make_unique<JumpZero>(rand); },
			[](int rand) { return std::make_unique<Halt>(); },
			[](int rand) { return std::make_unique<Copy>(); },
	};

	Machine result;
	result.addInstruction(std::make_unique<Immediate>(0));
	int n = std::abs(static_cast<int>((random() % (1ul << 4))));
	for (int i = 0; i < n; ++i) {
		auto instruction = constructors[std::abs(static_cast<int>((random() % 9)))](static_cast<int>(random() % 16));
		result.addInstruction(std::move(instruction));
	}
	return result;
}

int main() {
	// 3*x+5

	std::mt19937 random{0};

	constexpr std::size_t MACHINE_COUNT{50000};
	std::array<Machine, MACHINE_COUNT> machines;

	// generate machines, mt19937 is hard to use in multiple threads
	for(auto& machine : machines) {
		machine = genMachine(random);
	}

	constexpr int MAX_CYCLES{100};
	constexpr int INITIAL_VALUES{10000};
	int invalidCount{};
	std::array<int, MACHINE_COUNT> dists{};

	auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for default(none) reduction(+:invalidCount) shared(dists,machines)
	for (int i = 0; i < machines.size(); ++i) {
		auto& machine = machines[i];

		for (int j{0}; j < INITIAL_VALUES; ++j) {
			int initialValue = j;
			machine.setInitial(initialValue);
			try {
				for (int k{0}; k < MAX_CYCLES && machine.running(); ++k) {
					machine.execute(machine.fetch());
				}

				if(machine.running()) {
					dists[i] = std::numeric_limits<int>::max();
					++invalidCount;
					break;
				} else {
					int result = machine.getTop();
					int y = 3 * initialValue + 5;
					dists[i] += std::abs(result - y);
				}
			} catch (const std::runtime_error& error) {
				dists[i] = std::numeric_limits<int>::max();
				++invalidCount;
				break;
			}
		}
		if(dists[i] < std::numeric_limits<int>::max()) dists[i] /= INITIAL_VALUES;
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << "Finished execution after " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";
	
	int bestIndex = -1;
	int bestDist = std::numeric_limits<int>::max();
	for (int i = 0; i < machines.size(); ++i) {
		if (dists[i] < bestDist) {
			bestDist = dists[i];
			bestIndex = i;
		}
	}
	assert(bestIndex >= 0);
	
	std::cout << "Finished finding best\n";

	for (int i{0}; i < MACHINE_COUNT; ++i) {
		if(dists[i] < std::numeric_limits<int>::max()) {
			std::cout << machines[i].to_string() << '\n';
		}
	}

	std::cout << bestIndex << " of " << MACHINE_COUNT - invalidCount << " valid machines\n" << machines[bestIndex].to_string();

	return 0;
}
