//
// Created by jakob on 07.08.21.
//

#include <stdexcept>
#include "Div.hpp"

void Div::execute(std::stack<int>& stack, int& ip) {
	if(stack.size() < 2){
		throw std::runtime_error{"Too few variables on stack"};
	}

	int top = stack.top();
	stack.pop();

	if(top == 0) {
		throw std::runtime_error{"Division by zero"};
	}

	stack.top() /= top;

	Instruction::execute(stack, ip);
}

std::string Div::to_string() {
	return "Div";
}
