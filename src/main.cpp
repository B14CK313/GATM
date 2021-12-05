#include <chrono>
#include <cassert>
#include <iostream>
#include <random>
#include <functional>
#include <limits>
#include <omp.h>
#include "Machine.hpp"

Machine genMachine(std::mt19937 &random) {
	Machine result;
	int n = std::abs(static_cast<int>((random() % (1ul << 4))));
	for(int i = 0; i < n; ++i) {
		auto instruction = std::abs(static_cast<int>(random() % 9));
		result.pushCode(instruction);
		if(instruction == Machine::Instruction::JumpZero) {
			result.pushCode(std::abs(static_cast<int>(random() % 16)));
		} else if(instruction == Machine::Instruction::Immediate) {
			result.pushCode(static_cast<int>(random() % 16));
		}
	}
	return result;
}

struct Instances {
	Machine *machines;
	size_t machineCount;
};

struct MachineStats {
	int machineIndex;
	Machine *machine;
	int dist;

	MachineStats(int machineIndex, Machine *machine, int dist) : machineIndex(machineIndex), machine(machine),
																 dist(dist) {}
};

struct MachineRanking {
	std::vector<MachineStats> bestMachines;
	/** Maximum number of machines that are stored as "best" */
	size_t maxMachines;

	explicit MachineRanking(size_t maxMachines) : maxMachines(maxMachines) {}

	/**
	 * Insert a MachineStats object into the leaderboard if it is good enough or the number of machines is less than
	 * maxMachines.
	 * @param newStats This object will be compared.
	 */
	void insert(MachineStats newStats) {
		int insertIndex = bestMachines.size();
		for(int i = 0; i < bestMachines.size(); ++i) {
			MachineStats &stats = bestMachines[i];
			// smaller is better
			if(stats.dist > newStats.dist) {
				insertIndex = i;
				break;
			}
		}

		bestMachines.insert(bestMachines.begin() + insertIndex, newStats);
		if(bestMachines.size() > maxMachines) bestMachines.pop_back();
	}
};

MachineRanking run(Instances &instances) {
	constexpr int MAX_CYCLES{10};
	constexpr int INITIAL_VALUES{1'000};
	constexpr int MAX_BEST_MACHINES{10};

	MachineRanking result(MAX_BEST_MACHINES);
	int invalidCount{};

#pragma omp parallel for default(none) reduction(+:invalidCount) shared(result, instances)
	for(int i = 0; i < instances.machineCount; ++i) {
		auto &machine = instances.machines[i];

		int dist = 0;

		for(int j{0}; j < INITIAL_VALUES; ++j) {
			int x = j - INITIAL_VALUES / 2;
			machine.setInitial(x);
			try {
				for(int k{0}; k < MAX_CYCLES && machine.running(); ++k) {
					machine.step();
				}

				if(machine.running()) {
					++invalidCount;
					goto outer;
				} else {
					int machineResult = machine.top();
					int y = 3 * x + 5;
					auto diff = std::abs(machineResult - y);
					if(diff > 100'000) {
						++invalidCount;
						goto outer;
					}
					dist += diff;
				}
			} catch(const std::runtime_error &error) {
				++invalidCount;
				goto outer;
			}
		}
		if(dist < std::numeric_limits<int>::max()) dist /= INITIAL_VALUES;

#pragma omp critical
		result.insert(MachineStats(i, &machine, dist));

		outer:;
	}

	return result;
}

int main() {
	// 3*x+5

	std::mt19937 random{0};

	constexpr std::size_t MACHINE_COUNT{10'000};
	auto *machines = new Machine[MACHINE_COUNT];

	for(int i = 0; i < MACHINE_COUNT; ++i) {
		machines[i] = genMachine(random);
	}

	auto start = std::chrono::high_resolution_clock::now();
	Instances instances = {
			.machines = machines,
			.machineCount = MACHINE_COUNT
	};
	MachineRanking ranking = run(instances);
	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << "Finished execution after "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";

	std::cout << "\nevaluation:\n";
	for(int i = 0; i < ranking.bestMachines.size(); ++i) {
		auto &stats = ranking.bestMachines[i];
		std::cout << "place #" << (i + 1) << " occupied by machine " << stats.machineIndex << " with dist "
				  << stats.dist
				  << "\n";
		std::cout << stats.machine->to_string() << '\n';
	}

	delete[] machines;

	return 0;
}
