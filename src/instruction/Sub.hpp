//
// Created by jakob on 07.08.21.
//

#ifndef GATM_SUB_HPP
#define GATM_SUB_HPP


#include "Instruction.hpp"

class Sub : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};


#endif //GATM_SUB_HPP
