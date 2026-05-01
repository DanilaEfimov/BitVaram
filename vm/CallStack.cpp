#include "CallStack.h"
#include "exceptions/ExStackOverflow.h"
#include "exceptions/ExStackUnderflow.h"

CallStack::CallStack(size_t stack_size)
	: calls(), capacity(stack_size)
{
	this->calls.reserve(stack_size);
}

bool CallStack::empty() const noexcept
{
	return this->calls.empty();
}

size_t CallStack::size() const noexcept
{
	return this->calls.size();
}

CallStack::ip CallStack::top() const
{
	if (this->empty())
		throw ExStackUnderflow("call stack underflow");

	return this->calls.back();
}

CallStack::ip CallStack::pop()
{
	if (this->empty())
		throw ExStackUnderflow("call stack underflow");

	ip pointer = this->calls.back();
	this->calls.pop_back();
	return pointer;
}

void CallStack::push(ip pointer)
{
	if (this->size() >= this->capacity)
		throw ExStackOverflow("call stack overflow");

	this->calls.push_back(pointer);
}
