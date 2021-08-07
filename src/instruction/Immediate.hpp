//
// Created by jakob on 07.08.21.
//

#ifndef GATM_IMMEDIATE_HPP
#define GATM_IMMEDIATE_HPP

#include "Instruction.hpp"

class Immediate : public Instruction {
public:
	explicit Immediate(int value);
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
	void set_value(int value);

private:
	int value_;
};


#endif //GATM_IMMEDIATE_HPP
