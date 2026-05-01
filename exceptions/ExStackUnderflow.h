#pragma once

#include "StackException.h"

#include <stdexcept>

class ExStackUnderflow : public StackException
{
public:
	explicit ExStackUnderflow(const char* msg);
};

