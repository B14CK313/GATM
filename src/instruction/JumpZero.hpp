//
// Created by jakob on 07.08.21.
//

#ifndef GATM_JUMPZERO_HPP
#define GATM_JUMPZERO_HPP


#include "Instruction.hpp"

class JumpZero : public Instruction {
public:
	JumpZero(int address);

	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
private:
	int address_;
};


#endif //GATM_JUMPZERO_HPP
