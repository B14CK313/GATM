#include <cassert>
#include <iostream>
#include <random>
#include <functional>
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
		auto instruction = constructors[std::abs(static_cast<int>((random() % 9)))](static_cast<int>(random() % 1024));
		result.addInstruction(std::move(instruction));
	}
	return result;
}

int main() {
	// 3*x+5

	int invalidCount{};
	std::mt19937 random{0}; // NOLINT(cert-msc51-cpp)

	constexpr std::size_t machineCount{10000};
	std::array<Machine, machineCount> machines;
	std::array<int, machineCount> dists{};

#pragma omp parallel for default(none) shared(machines, random, dists, invalidCount)
	for (int i = 0; i < machines.size(); ++i) {
		auto& machine = machines[i];
		machine = genMachine(random);

		for (int j{0}; j < 10000; ++j) {
			int initialValue = j;
			machine.setInitial(initialValue);
			try {
				while (machine.running()) {
					machine.execute(machine.fetch());
				}

				int result = machine.getTop();
				int y = 3 * initialValue + 5;
				dists[i] += std::abs(result - y);
			} catch (const std::runtime_error& error) {
				dists[i] = INT_MAX;
				++invalidCount;
				break;
			}
		}
		if(dists[i] < INT_MAX) dists[i] /= 100;
	}

	int bestIndex = -1;
	int bestDist = INT_MAX;
	for (int i = 0; i < machines.size(); ++i) {
		if (dists[i] < bestDist) {
			bestDist = dists[i];
			bestIndex = i;
		}
	}
	assert(bestIndex >= 0);

	for (int i{0}; i < machineCount; ++i) {
		if(dists[i] < INT_MAX) {
			std::cout << machines[i].to_string() << '\n';
		}
	}

	std::cout << bestIndex << " of " << machineCount - invalidCount << " valid machines\n" << machines[bestIndex].to_string();

	return 0;
}
