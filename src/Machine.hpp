//
// Created by jakob on 06.08.21.
//

#ifndef GATM_MACHINE_HPP
#define GATM_MACHINE_HPP


#include <array>
#include <vector>
#include <stack>
#include <memory>
#include "instruction/Instruction.hpp"

class Machine {
public:
	[[nodiscard]] bool running() const;

	Instruction* fetch();

	void execute(Instruction* instruction);

	void addInstruction(std::unique_ptr<Instruction> instruction);

	int getTop();

	void setInitial(int i);

	std::string to_string();
private:
	std::stack<int> stack_{};
	std::vector<std::unique_ptr<Instruction>> instructions_{};
	int ip_;
};


#endif //GATM_MACHINE_HPP
