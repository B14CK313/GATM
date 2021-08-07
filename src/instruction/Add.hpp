//
// Created by jakob on 06.08.21.
//

#ifndef GATM_ADD_HPP
#define GATM_ADD_HPP


#include "Instruction.hpp"

class Add : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};


#endif //GATM_ADD_HPP
