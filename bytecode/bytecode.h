#pragma once

#include <cstdint>

namespace bytecode {

	enum Op : uint8_t
	{
		PUSH_CONST,
		LOAD_LOCAL,
		STORE_LOCAL,
		POP,
		DUP,
		SWAP,
		// arithmetic
		ADD,
		SUB,
		MUL,
		DIV,
		NEG,
		// comparing
		EQ,
		NE,
		LT,
		GT,
		LE,
		GE,
		// control flow
		JMP,
		JZ,
		JNZ,
		HALT,
	};

}