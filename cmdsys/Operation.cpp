#include "Operation.h"

Operation::Operation(Op op)
	: op(op)
{}

Op Operation::type() const noexcept
{
	return this->op;
}
