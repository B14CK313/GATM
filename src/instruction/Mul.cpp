//
// Created by jakob on 07.08.21.
//

#include <stdexcept>
#include "Mul.hpp"

void Mul::execute(std::stack<int>& stack, int& ip) {
	if(stack.size() < 2){
		throw std::runtime_error{"Too few variables on stack"};
	}

	int top = stack.top();
	stack.pop();
	stack.top() *= top;

	Instruction::execute(stack, ip);
}

std::string Mul::to_string() {
	return "Mul";
}
