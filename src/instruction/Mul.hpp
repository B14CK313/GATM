//
// Created by jakob on 07.08.21.
//

#ifndef GATM_MUL_HPP
#define GATM_MUL_HPP


#include "Instruction.hpp"

class Mul : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};


#endif //GATM_MUL_HPP
