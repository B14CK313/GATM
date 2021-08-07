//
// Created by jakob on 07.08.21.
//

#include <stdexcept>
#include <sstream>
#include "JumpZero.hpp"

void JumpZero::execute(std::stack<int>& stack, int& ip) {
	if(stack.empty()){
		throw std::runtime_error{"Too few variables on stack"};
	}

	int top = stack.top();
	stack.pop();
	if(!top) {
		ip = address_;
	} else {
		Instruction::execute(stack, ip);
	}
}

JumpZero::JumpZero(int address) : address_(address) {}

std::string JumpZero::to_string() {
	std::stringstream ss{};
	ss << "JumpZero ";
	ss << address_;
	return ss.str();
}
