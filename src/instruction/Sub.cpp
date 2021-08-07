//
// Created by jakob on 07.08.21.
//

#include <stdexcept>
#include "Sub.hpp"

void Sub::execute(std::stack<int>& stack, int& ip) {
	if(stack.size() < 2){
		throw std::runtime_error{"Too few variables on stack"};
	}

	int top = stack.top();
	stack.pop();
	stack.top() -= top;

	Instruction::execute(stack, ip);
}

std::string Sub::to_string() {
	return "Sub";
}
