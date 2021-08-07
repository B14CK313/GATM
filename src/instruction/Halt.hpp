//
// Created by jakob on 07.08.21.
//

#ifndef GATM_HALT_HPP
#define GATM_HALT_HPP

#include "Instruction.hpp"

class Halt : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};


#endif //GATM_HALT_HPP
