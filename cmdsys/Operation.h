#pragma once

#include "../Stack.h"
#include "../bytecode/bytecode.h"

using namespace bytecode;

class Operation
{
	Op op;

public:
	Operation(Op op);

	virtual void execute(Stack& stack) = 0;

	Op type() const noexcept;
};

