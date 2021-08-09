//
// Created by jakob on 07.08.21.
//

#pragma once

#include "Instruction.hpp"

class Immediate : public Instruction {
public:
	explicit Immediate(int value);

	void execute(std::stack<int>& stack, int& ip) override;

	void set_value(int value);

	std::string to_string() override;
private:
	int value_;
};

