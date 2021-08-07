//
// Created by jakob on 07.08.21.
//

#include <sstream>
#include "Immediate.hpp"

void Immediate::execute(std::stack<int>& stack, int& ip) {
	stack.push(value_);

	Instruction::execute(stack, ip);
}

Immediate::Immediate(int value) : value_(value) {}

void Immediate::set_value(int value) {
	value_ = value;
}

std::string Immediate::to_string() {
	std::stringstream ss{};
	ss << "Immediate ";
	ss << value_;
	return ss.str();
}
