//
// Created by jakob on 07.08.21.
//

#include "Halt.hpp"

void Halt::execute(std::stack<int>& stack, int& ip) {
	ip = -1;
}

std::string Halt::to_string() {
	return "Halt";
}
