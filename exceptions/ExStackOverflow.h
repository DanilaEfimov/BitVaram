#pragma once

#include "StackException.h"

class ExStackOverflow : public StackException
{
public:
	explicit ExStackOverflow(const char* msg);
};

