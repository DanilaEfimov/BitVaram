#pragma once

#include "TypeException.h"

class ExTypeMismatch : public TypeException
{
public:
	ExTypeMismatch(const char* msg);
};

