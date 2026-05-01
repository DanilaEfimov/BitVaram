#pragma once

#include "Value.h"

#include <vector>

struct Stack
{
private:
	std::vector<Value> items;
	size_t capacity;

public:
	Stack(size_t stack_size);

	Value pop();
	void push(Value value);

	size_t size() const noexcept;
	size_t maxSize() const noexcept;
	bool empty() const noexcept;
};

