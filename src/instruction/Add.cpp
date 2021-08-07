//
// Created by jakob on 06.08.21.
//

#include <stdexcept>
#include <stack>
#include "Add.hpp"

void Add::execute(std::stack<int>& stack, int& ip) {
	if(stack.size() < 2){
		throw std::runtime_error{"Too few variables on stack"};
	}

	int top = stack.top();
	stack.pop();
	stack.top() += top;

	Instruction::execute(stack, ip);
}

std::string Add::to_string() {
	return "Add";
}
