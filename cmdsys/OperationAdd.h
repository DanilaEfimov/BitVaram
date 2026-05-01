#pragma once

#include "Operation.h"

class OperationAdd : public Operation
{
public:
	OperationAdd();

	void execute(Stack& stack) override;
};

