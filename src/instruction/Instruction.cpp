//
// Created by jakob on 06.08.21.
//

#include "Instruction.hpp"

void Instruction::execute(std::stack<int>& stack, int& ip) {
	ip++;
}

std::string Instruction::to_string() {
	return "Undefined Instruction";
}
