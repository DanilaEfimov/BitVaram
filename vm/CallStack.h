#pragma once

#include <vector>

class CallStack
{
public:
	using ip = size_t;

private:
	std::vector<ip> calls;
	size_t capacity;

public:
	CallStack(size_t stack_size);

	bool empty() const noexcept;
	size_t size() const noexcept;

	ip top() const;
	ip pop();
	void push(ip pointer);
};

