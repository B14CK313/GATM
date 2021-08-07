//
// Created by jakob on 07.08.21.
//

#ifndef GATM_NOT_HPP
#define GATM_NOT_HPP


#include "Instruction.hpp"

class Not : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};


#endif //GATM_NOT_HPP
