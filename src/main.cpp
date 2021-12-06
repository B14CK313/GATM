#include <chrono>
#include <cassert>
#include <iostream>
#include <random>
#include <functional>
#include <limits>
#include <omp.h>
#include "Machine.hpp"

struct MachineStats {
	int machineIndex;
	Machine *machine;
	int dist;

	MachineStats(int machineIndex, Machine *machine, int dist) : machineIndex(machineIndex), machine(machine),
																 dist(dist) {}
};

struct MachineRanking {
	std::vector<MachineStats> bestMachines;
	/** Maximum number of machines that are stored as "best". */
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

Machine genMachine(std::mt19937 &random) {
	Machine result;
	int n = std::abs(static_cast<int>((random() % (1ul << 4))));
	for(int i = 0; i < n; ++i) {
		uint8_t instruction = std::abs(static_cast<int>(random() % 9));
		result.pushCode(instruction);
		if(instruction == Machine::Instruction::JumpZero) {
			result.pushCode(std::abs(static_cast<int>(random() % 16)));
		} else if(instruction == Machine::Instruction::Immediate) {
			result.pushCode(static_cast<int>(random() % 16));
		}
	}
	return result;
}

struct MutationStats {
	size_t totalMutations;
	float mutationsPerMachine;
};

struct Machines {
	Machine *machines;
	size_t machineCount;

	Machines(Machine *machines, size_t machineCount) : machines(machines), machineCount(machineCount) {}

	~Machines() {
		delete[] machines;
	}

	/**
	 * Fill the machines array with the best machines.
	 *
	 * @param ranking The machine ranking with the best machines.
	 */
	void fillFromRanking(MachineRanking ranking) const {
		// round the division up -> machineIndex / instanceCount < machineCount always holds
		auto instanceCount = ((machineCount + ranking.bestMachines.size() - 1) / ranking.bestMachines.size());
		for(int machineIndex = 0; machineIndex < machineCount; ++machineIndex) {
			machines[machineIndex] = *ranking.bestMachines[machineIndex / instanceCount].machine;
		}
	}

	/**
	 * Slightly mutate each machine according to the RNG.
	 *
	 * @return Statistics about the mutations.
	 */
	[[nodiscard]] MutationStats mutate() const {
		static std::mt19937 random{0};

		MutationStats result{};
		for(int i = 0; i < machineCount; ++i) {
			Machine &machine = machines[i];
			size_t mutations = machine.mutate(random);
			result.mutationsPerMachine += mutations;
			result.totalMutations += mutations;
		}
		result.mutationsPerMachine /= machineCount;

		return result;
	}
};

static Machines genMachines() {
	std::mt19937 random{0};

	constexpr std::size_t MACHINE_COUNT{50'000};
	auto *machines = new Machine[MACHINE_COUNT];

	for(int i = 0; i < MACHINE_COUNT; ++i) {
		machines[i] = genMachine(random);
	}

	return Machines(machines, MACHINE_COUNT);
}

std::pair<MachineRanking, int> run(Machines &machines) {
	constexpr int MAX_CYCLES{100};
	constexpr int INITIAL_VALUES{1'000};
	constexpr int MAX_BEST_MACHINES{100};

	MachineRanking result(MAX_BEST_MACHINES);
	int invalidCount{};

#pragma omp parallel for default(none) reduction(+:invalidCount) shared(result, machines)
	for(int i = 0; i < machines.machineCount; ++i) {
		auto &machine = machines.machines[i];

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
					if(diff > 1'000) {
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

	return {result, invalidCount};
}

int main() {
	// 3*x+5

	Machines machines = genMachines();
	auto start = std::chrono::high_resolution_clock::now();
	MachineRanking ranking(0);
	for(int iteration = 0; iteration < 100; ++iteration) {
		std::cout << "iteration " << iteration << " with " << machines.machineCount << " machines\n";

		auto[r, invalidCount] = run(machines);
		ranking = r;

		machines.fillFromRanking(ranking);
		MutationStats mutationStats = machines.mutate();

		std::cout << "\tinvalid machines: " << invalidCount << ", total mutations: " << mutationStats.totalMutations
				  << ", mutations/machine: "
				  << mutationStats.mutationsPerMachine << "\n";
	}
	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << "Finished execution after "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";

	std::cout << "\niteration result:\n";
	for(int i = 0; i < ranking.bestMachines.size(); ++i) {
		auto &stats = ranking.bestMachines[i];
		std::cout << "place #" << (i + 1) << " occupied by machine " << stats.machineIndex << " with dist "
				  << stats.dist
				  << "\n";
		std::cout << stats.machine->to_string() << '\n';
	}

	return 0;
}
