//
// Created by jakob on 07.08.21.
//

#include <stdexcept>
#include "Not.hpp"

void Not::execute(std::stack<int>& stack, int& ip) {
	if(stack.empty()){
		throw std::runtime_error{"Too few variables on stack"};
	}

	int top = stack.top();
	stack.pop();
	stack.push(top == 0);

	Instruction::execute(stack, ip);
}

std::string Not::to_string() {
	return "Not";
}
