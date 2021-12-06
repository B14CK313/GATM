//
// Created by jakob on 06.08.21.
//

#include <stdexcept>
#include <sstream>
#include "Machine.hpp"

bool Machine::running() const {
	return ip_ != -1;
}

void Machine::step() {
	if (ip_ < 0 || ip_ >= instructions_.size()) {
		throw std::runtime_error{"Invalid instruction pointer!"};
	}

	Instruction instr = instruction();
	switch (instr) {
		case Immediate:
			push(immediate());
			break;
		case Add:
			top() += pop();
			break;
		case Sub:
			top() -= pop();
			break;
		case Mul:
			top() *= pop();
			break;
		case Div:
			if(top() == 0) throw std::runtime_error{"Division by zero"};
			top() /= pop();
			break;
		case Not:
			top() = top() == 0;
			break;
		case JumpZero:
			if (pop() == 0) ip_ = immediate() - 1;
			break;
		case Copy:
			push(top());
			break;
		case Halt:
			ip_ = -1;
			break;
		default:
			throw std::runtime_error{"Instruction out of range"};
	}
}

void Machine::pushCode(int code) {
	instructions_.push_back(code);
}

Machine::Instruction Machine::getInstructionAt(int location) {
	return (Instruction)(instructions_[location] % INSTRUCTION_COUNT);
}

Machine::Instruction Machine::instruction() {
	if(ip_ >= instructions_.size()) {
		throw std::runtime_error{"Instruction pointer out of range"};
	}

	return getInstructionAt(ip_++);
}

int Machine::immediate() {
	if(ip_ >= instructions_.size()) {
		throw std::runtime_error{"Immediate expected"};
	}

	return instructions_[ip_++];
}

int& Machine::top() {
	if (stackPointer_ == -1) {
		throw std::runtime_error{"Stack is empty"};
	}
	if (stackPointer_ >= stack_.size()) {
		throw std::runtime_error{"Invalid state"};
	}
	return stack_[stackPointer_];
}

int Machine::pop() {
	if (stackPointer_ == -1) {
		throw std::runtime_error{"Stack is empty"};
	}
	if (stackPointer_ >= stack_.size()) {
		throw std::runtime_error{"Invalid state"};
	}
	return stack_[stackPointer_--];
}

void Machine::push(int i) {
	if (stackPointer_ + 1 >= stack_.size()) {
		throw std::runtime_error{"Stack overflow"};
	}
	stack_[++stackPointer_] = i;
}

static uint8_t flipBit(uint8_t num, unsigned bitIndex) {
	uint8_t mask = ~(1u << bitIndex);
	uint8_t result = (num & mask) | ((!((num >> bitIndex) & 1u)) << bitIndex);
	return result;
}

size_t Machine::mutate(std::mt19937 &random) {
	size_t mutations = 0;
	for(uint8_t &instr : instructions_) {
		for(unsigned bitIndex = 0; bitIndex < 8; ++bitIndex) {
			// todo: remove this magic number
			if(std::uniform_real_distribution<>(0, 1)(random) < 0.06) {
				instr = flipBit(instr, bitIndex);
				++mutations;
			}
		}
	}
	return mutations;
}

void Machine::setInitial(int i) {
	ip_ = 0;
	stackPointer_ = -1;
	push(i);
}

std::string Machine::to_string() {
	std::stringstream ss{};
	ss << "Machine:\n";
	for (int i{0}; i < instructions_.size(); ++i) {
		ss << i << ": ";
		switch(instructions_[i] % INSTRUCTION_COUNT){
			case Immediate:
				ss << "Immediate{" << (int)instructions_[++i] << "}";
				break;
			case Add:
				ss << "Add";
				break;
			case Sub:
				ss << "Sub";
				break;
			case Mul:
				ss << "Mul";
				break;
			case Div:
				ss << "Div";
				break;
			case Not:
				ss << "Not";
				break;
			case JumpZero:
				ss << "JumpZero{" << (int)instructions_[++i] << "}";
				break;
			case Copy:
				ss << "Copy";
				break;
			case Halt:
				ss << "Halt";
				break;
		}
		ss << "\n";
	}
	return ss.str();
}
