//
// Created by jakob on 07.08.21.
//

#include <stdexcept>
#include "Copy.hpp"

void Copy::execute(std::stack<int>& stack, int& ip) {
	if(stack.empty()){
		throw std::runtime_error{"Too few variables on stack"};
	}

	stack.push(stack.top());

	Instruction::execute(stack, ip);
}

std::string Copy::to_string() {
	return "Copy";
}
