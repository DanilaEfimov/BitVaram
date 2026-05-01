#include "Stack.h"
#include "exceptions/ExStackOverflow.h"
#include "exceptions/ExStackUnderflow.h"

Stack::Stack(size_t stack_size)
	: capacity(stack_size)
{
	this->items.reserve(stack_size);
}

Value Stack::pop()
{
	if (this->empty())
		throw ExStackUnderflow("stack underflow");

	Value value = std::move(this->items.back());
	this->items.pop_back();
	return value;
}

void Stack::push(Value value)
{
	if (this->size() == this->items.capacity())
		throw ExStackOverflow("stack overflow");

	this->items.push_back(std::move(value));
}

size_t Stack::size() const noexcept
{
	return this->items.size();
}

size_t Stack::maxSize() const noexcept
{
	return this->capacity;
}

bool Stack::empty() const noexcept
{
	return this->items.empty();
}
