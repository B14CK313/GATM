#include <chrono>
#include <cassert>
#include <iostream>
#include <random>
#include <functional>
#include <limits>
#include <omp.h>
#include "Machine.hpp"

Machine genMachine(std::mt19937& random) {
	Machine result;
	int n = std::abs(static_cast<int>((random() % (1ul << 4))));
	for (int i = 0; i < n; ++i) {
		auto instruction = std::abs(static_cast<int>(random() % 9));
		result.pushCode(instruction);
		if (instruction == Machine::Instruction::JumpZero) {
			result.pushCode(std::abs(static_cast<int>(random() % 16)));
		} else if (instruction == Machine::Instruction::Immediate) {
			result.pushCode(static_cast<int>(random() % 16));
		}
	}
	return result;
}

int main() {
	// 3*x+5

	std::mt19937 random{0};

	constexpr std::size_t MACHINE_COUNT{1'000'000};
	auto* machines = new Machine[MACHINE_COUNT];

	for (int i = 0; i < MACHINE_COUNT; ++i) {
		machines[i] = genMachine(random);
	}

	constexpr int MAX_CYCLES{100};
	constexpr int INITIAL_VALUES{10'000};
	int invalidCount{};
	auto* dists = new std::pair<int, int>[MACHINE_COUNT];

	auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for default(none) reduction(+:invalidCount) shared(dists, machines, MACHINE_COUNT)
	for (int i = 0; i < MACHINE_COUNT; ++i) {
		auto& machine = machines[i];

		dists[i] = (std::pair{i, 0});

		for (int j{0}; j < INITIAL_VALUES; ++j) {
			int x = j - INITIAL_VALUES/2;
			machine.setInitial(x);
			try {
				for (int k{0}; k < MAX_CYCLES && machine.running(); ++k) {
					machine.step();
				}

				if (machine.running()) {
					dists[i].second = std::numeric_limits<int>::max();
					++invalidCount;
					goto outer;
				} else {
					int result = machine.top();
					int y = 3*x+5;
					auto diff = std::abs(result - y);
					if (diff > 100'000) {
						dists[i].second = std::numeric_limits<int>::max();
						++invalidCount;
						goto outer;
					}
					dists[i].second += diff;
				}
			} catch (const std::runtime_error& error) {
				dists[i].second = std::numeric_limits<int>::max();
				++invalidCount;
				goto outer;
			}
		}
		if (dists[i].second < std::numeric_limits<int>::max()) dists[i].second /= INITIAL_VALUES;
		outer:;
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << "Finished execution after "
	          << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";

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

//	for (int i{0}; i < MACHINE_COUNT; ++i) {
//		if (dists[i].second < std::numeric_limits<int>::max()) {
//			std::cout << machines[i].to_string() << '\n';
//		}
//	}

	std::sort(dists, dists+MACHINE_COUNT, [](const auto& p1, const auto& p2) { return p1.second < p2.second; });

	for (int i{0}; i < MACHINE_COUNT; ++i) {
		 const auto& dist = dists[i];
		if (dist.second > bestDist) break;
		std::cout << "Id: " << dist.first << " : " << machines[dist.first].to_string() << '\n';
	}

	delete[] dists;
	delete[] machines;

	return 0;
}
