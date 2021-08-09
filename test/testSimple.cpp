//
// Created by jakob on 07.08.21.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <cassert>
#include <memory>
#include "../src/instruction/Instruction.hpp"
#include "../src/Machine.hpp"

TEST_CASE("Simple machine test") {
	// 3*x+5

	// Arrange
	int x = 4;

	Machine machine{};
	machine.addInstruction(std::make_unique<Immediate>(x));
	machine.addInstruction(std::make_unique<Immediate>(3));
	machine.addInstruction(std::make_unique<Mul>());
	machine.addInstruction(std::make_unique<Immediate>(5));
	machine.addInstruction(std::make_unique<Add>());
	machine.addInstruction(std::make_unique<Halt>());

	// Act
	while(machine.running()) {
		machine.execute(machine.fetch());
	}

	// Assert
	assert(machine.getTop() == 17);
}

TEST_CASE("Test jump") {
	/*
	 * while(x!=0) {x = x - 2;}
	 *
	 * 0 IMM x
	 * 1 B: COPY
	 * 2 JZ A
	 * 3 IMM 2
	 * 4 SUB
	 * 5 IMM 0
	 * 6 JZ B
	 * 7 A: HALT
	 */

	// Arrange
	int x = 4;

	Machine machine{};
	machine.addInstruction(std::make_unique<Immediate>(x));
	machine.addInstruction(std::make_unique<Copy>());
	machine.addInstruction(std::make_unique<JumpZero>(7));
	machine.addInstruction(std::make_unique<Immediate>(2));
	machine.addInstruction(std::make_unique<Sub>());
	machine.addInstruction(std::make_unique<Immediate>(0));
	machine.addInstruction(std::make_unique<JumpZero>(1));
	machine.addInstruction(std::make_unique<Halt>());

	// Act
	while(machine.running()) {
		machine.execute(machine.fetch());
	}

	// Assert
	assert(machine.getTop() == 0);
}
