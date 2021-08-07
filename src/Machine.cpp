//
// Created by jakob on 06.08.21.
//

#include <stdexcept>
#include <sstream>
#include "Machine.hpp"

bool Machine::running() const {
	return ip_ != -1;
}

Instruction* Machine::fetch() {
	if(ip_ < instructions_.size()) {
		return instructions_[ip_].get();
	} else {
		throw std::runtime_error{"Invalid instruction pointer!"};
	}
}

void Machine::execute(Instruction* instruction) {
	instruction->execute(stack_, ip_);
}

void Machine::addInstruction(std::unique_ptr<Instruction> instruction) {
	instructions_.push_back(std::move(instruction));
}

int Machine::getTop() {
	if(stack_.empty()) {
		throw std::runtime_error{"Stack is empty"};
	}
	return stack_.top();
}

void Machine::setInitial(int i) {
	dynamic_cast<Immediate*>(instructions_[0].get())->set_value(i);
}

std::string Machine::to_string() {
	std::stringstream ss{};
	ss << "Machine:\n";
	for (int i{0}; i < instructions_.size(); ++i) {
		ss << i << ": ";
		ss << instructions_[i]->to_string();
		ss << "\n";
	}
	return ss.str();
}
