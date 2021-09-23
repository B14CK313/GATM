//
// Created by jakob on 06.08.21.
//

#pragma once

#include <array>
#include <vector>
#include <stack>
#include <memory>
#include "instruction/Instruction.hpp"

class Machine {
public:
	Instruction* fetch();

	void execute(Instruction* instruction);

	void addInstruction(std::unique_ptr<Instruction> instruction);

	void setInitial(int i);

	[[nodiscard]] bool running() const;

	int getTop();

	std::string to_string();
private:
	std::stack<int> stack_{};
	std::vector<std::unique_ptr<Instruction>> instructions_{};
	int ip_{0};
};

