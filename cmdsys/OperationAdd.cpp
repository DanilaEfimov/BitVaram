#include "OperationAdd.h"
#include "../Converter.h"
#include "../IntArithmetic.h"
#include "../FloatArythmetic.h"
#include "../exceptions/ExTypeMismatch.h"

OperationAdd::OperationAdd()
	: Operation(bytecode::ADD)
{}

void OperationAdd::execute(Stack& stack)
{
	Value right = stack.pop();
	Value left = stack.pop();

	auto ltype = left.getType();
	auto rtype = right.getType();
	if (!bv::arythmeticType(ltype) || !bv::arythmeticType(rtype))
		throw ExTypeMismatch("OperationAdd::type mismatch");

	Value ret;

	if (ltype != rtype) {
		Converter::float_cast(left);
		Converter::float_cast(right);
		
		ret = FloatArythmetic::add(left, right);
		stack.push(ret);
		return;
	}

	if (ltype == bv::Type::Int)
		ret = IntArythmetic::add(left, right);
	else
		ret = FloatArythmetic::add(left, right);

	stack.push(ret);
}
