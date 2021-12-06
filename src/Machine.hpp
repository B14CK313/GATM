//
// Created by jakob on 06.08.21.
//

#pragma once

#include <array>
#include <vector>
#include <stack>
#include <memory>
#include <random>

class Machine {
public:
	enum {INSTRUCTION_COUNT = 9};
	enum Instruction : uint8_t {
		Immediate, Add, Sub, Mul, Div, Not, JumpZero, Copy, Halt
	};

	void step();

	void pushCode(int code);

	void setInitial(int i);

	[[nodiscard]] bool running() const;

	Instruction getInstructionAt(int location);

	Instruction instruction();

	int immediate();

	int &top();

	int pop();

	void push(int i);

	size_t mutate(std::mt19937 &random);

	std::string to_string();

private:
	static constexpr int STACK_SIZE{16};
	std::array<int, STACK_SIZE> stack_{};
	int stackPointer_ = -1;
	std::vector<uint8_t> instructions_{};
	int ip_{0};
};

