//
// Created by jakob on 07.08.21.
//

#pragma once

#include "Instruction.hpp"

class JumpZero : public Instruction {
public:
	explicit JumpZero(int address);

	void execute(std::stack<int>& stack, int& ip) override;

	std::string to_string() override;
private:
	int address_;
};

