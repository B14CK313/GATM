//
// Created by jakob on 07.08.21.
//

#ifndef GATM_COPY_HPP
#define GATM_COPY_HPP

#include "Instruction.hpp"

class Copy : public Instruction {
public:
	void execute(std::stack<int>& stack, int& ip) override;
	std::string to_string() override;
};


#endif //GATM_COPY_HPP
