#pragma once

#include <stdexcept>

class StackException : public std::exception
{
public:
	explicit StackException(const char* msg);
};

