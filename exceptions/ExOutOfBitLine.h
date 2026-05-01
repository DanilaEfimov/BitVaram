#pragma once

#include "../BitLineExeption.h"

class ExOutOfBitLine : public BitLineExeption
{
public:
	explicit ExOutOfBitLine(const char* msg);
};

