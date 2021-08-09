//
// Created by jakob on 06.08.21.
//

#pragma once

#include "Instruction.hpp"

class Add : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};

