//
// Created by jakob on 07.08.21.
//

#ifndef GATM_DIV_HPP
#define GATM_DIV_HPP


#include "Instruction.hpp"

class Div : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};


#endif //GATM_DIV_HPP
