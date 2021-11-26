//
// Created by jakob on 06.08.21.
//

#pragma once

#include <array>
#include <vector>
#include <stack>
#include <memory>

class Machine {
public:
	enum Instruction {
		Immediate, Add, Sub, Mul, Div, Not, JumpZero, Copy, Halt
	};

	void step();

	void pushCode(int code);

	void setInitial(int i);

	[[nodiscard]] bool running() const;

	int immediate();

	int& top();
	int pop();
	void push(int i);

	std::string to_string();
private:
	static constexpr int STACK_SIZE{16};
	std::array<int, STACK_SIZE> stack_{};
	int stackPointer_ = -1;
	std::vector<int> instructions_{};
	int ip_{0};
};

