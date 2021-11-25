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
	int n = std::abs(static_cast<int>((random() % (1ul << 4))));
	for (int i = 0; i < n; ++i) {
		auto instruction = constructors[std::abs(static_cast<int>((random() % 9)))](static_cast<int>(random() % 16));
		result.addInstruction(std::move(instruction));
	}
	return result;
}

int main() {
	// 3*x+5

	std::mt19937 random{1};

	constexpr std::size_t MACHINE_COUNT{1'000'000};
	auto* machines = new Machine[MACHINE_COUNT];
	//machines.reserve(MACHINE_COUNT);

	// generate machines, mt19937 is hard to use in multiple threads
	//for(auto& machine : machines) {
	//	machine = genMachine(random);
	//}

	for (int i = 0; i < MACHINE_COUNT; ++i) {
		//machines.push_back(genMachine(random));
		machines[i] = genMachine(random);
	}

	constexpr int MAX_CYCLES{100};
	constexpr int INITIAL_VALUES{10'000};
	int invalidCount{};
	//std::array<int, MACHINE_COUNT> dists{};
	std::array<std::pair<int, int>, MACHINE_COUNT> dists{};

	auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for default(none) reduction(+:invalidCount) shared(dists,machines, MACHINE_COUNT, std::cout)
	for (int i = 0; i < MACHINE_COUNT; ++i) {
		auto& machine = machines[i];

		dists[i] = std::pair{i, 0};

		for (int j{0}; j < INITIAL_VALUES; ++j) {
			int initialValue = j;
			machine.setInitial(initialValue);
			try {
				for (int k{0}; k < MAX_CYCLES && machine.running(); ++k) {
					machine.execute(machine.fetch());
				}

				if(machine.running()) {
					dists[i].second = std::numeric_limits<int>::max();
					++invalidCount;
					goto outer;
				} else {
					int result = machine.getTop();
					int y = initialValue;
					auto diff = std::abs(result - y);
					if(diff > 1000) {
						dists[i].second = std::numeric_limits<int>::max();
						++invalidCount;
						goto outer;
					}
					dists[i].second += diff;
					//std::cout << "machineIndex: " << i << "initialValue: " << initialValue << " diff: " << diff << std::endl;
				}
			} catch (const std::runtime_error& error) {
				dists[i].second = std::numeric_limits<int>::max();
				++invalidCount;
				goto outer;
			}
		}
		if(dists[i].second < std::numeric_limits<int>::max()) dists[i].second /= INITIAL_VALUES;
outer:;
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << "Finished execution after " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";
	
	int bestIndex = -1;
	int bestDist = std::numeric_limits<int>::max();
	for (int i = 0; i < MACHINE_COUNT; ++i) {
		if (dists[i].second < bestDist) {
			bestDist = dists[i].second;
			bestIndex = i;
		}
	}
	assert(bestIndex >= 0);
	
	std::cout << "Finished finding best\n";

	for (int i{0}; i < MACHINE_COUNT; ++i) {
		if(dists[i].second < std::numeric_limits<int>::max()) {
			//std::cout << machines[i].to_string() << '\n';
		}
	}

	std::sort(dists.begin(), dists.end(), [](const auto& p1, const auto& p2){return p1.second < p2.second;});

	for (int i = 0; i < 200; ++i) {
		std::cout << "id: " << dists[i].first << " dist: " << dists[i].second << std::endl;
	}
	
	for (int i = 0; i < dists.size(); ++i) {
		if(dists[i].second > bestDist) break;
		std::cout << "Machine with id: " << dists[i].first << " : " << machines[dists[i].first].to_string() << '\n';
	}

	//std::cout << bestIndex << " of " << MACHINE_COUNT - invalidCount << " valid machines\n" << machines[bestIndex].to_string();

	std::cout << sizeof(Machine) << " " << sizeof(machines) << '\n';

	delete[] machines;

	return 0;
}
