//
// Created by jakob on 07.08.21.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <memory>
#include "../src/Machine.hpp"

void runMachine(Machine& machine) {
	while(machine.running()) {
		machine.step();
	}
}

TEST_CASE("Simple machine test") {
	// 3*x+5

	// Arrange
	int x = 4;

	Machine machine{};
	machine.pushCode(Machine::Immediate);
	machine.pushCode(3);
	machine.pushCode(Machine::Mul);
	machine.pushCode(Machine::Immediate);
	machine.pushCode(5);
	machine.pushCode(Machine::Add);
	machine.pushCode(Machine::Halt);

	machine.setInitial(x);

	// Act
	runMachine(machine);

	// Assert
	int stackTop = machine.top();
	CHECK(stackTop == 17);
}

TEST_CASE("Test jump") {
	/*
	 * while(x!=0) {x = x - 2;}
	 *
	 *  0 B: COPY
	 *  1 JZ A
	 *  3 IMM 2
	 *  5 SUB
	 *  6 IMM 0
	 *  8 JZ B
	 * 10 A: HALT
	 */

	// Arrange
	int x = 4;

	Machine machine{};
	/*  0 */ machine.pushCode(Machine::Copy);
	/*  1 */ machine.pushCode(Machine::JumpZero);
	/*  2 */ machine.pushCode(10);
	/*  3 */ machine.pushCode(Machine::Immediate);
	/*  4 */ machine.pushCode(2);
	/*  5 */ machine.pushCode(Machine::Sub);
	/*  6 */ machine.pushCode(Machine::Immediate);
	/*  7 */ machine.pushCode(0);
	/*  8 */ machine.pushCode(Machine::JumpZero);
	/*  9 */ machine.pushCode(0);
	/* 10 */ machine.pushCode(Machine::Halt);

	machine.setInitial(x);

	// Act
	runMachine(machine);

	// Assert
	CHECK(machine.top() == 0);
}
