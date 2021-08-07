//
// Created by jakob on 06.08.21.
//

#ifndef GATM_INSTRUCTION_HPP
#define GATM_INSTRUCTION_HPP


#include <stack>
#include <string>

class Instruction {
public:
	virtual void execute(std::stack<int>& stack, int& ip);
	virtual std::string to_string();
};

#include "Immediate.hpp"
#include "Add.hpp"
#include "Sub.hpp"
#include "Mul.hpp"
#include "Div.hpp"
#include "Not.hpp"
#include "JumpZero.hpp"
#include "Halt.hpp"
#include "Copy.hpp"


#endif //GATM_INSTRUCTION_HPP
