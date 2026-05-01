#pragma once

#include <stdexcept>

class TypeException : std::exception
{
public:
	TypeException(const char* msg);
};

