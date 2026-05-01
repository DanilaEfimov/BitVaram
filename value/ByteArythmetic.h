#pragma once

#include "Value.h"
#include "Config.h"

class ByteArythmetic
{
public:
	static constexpr size_t signbit = Config::intSize * 8 - 1;

	static bool getbit(const Value& val, size_t idx);
	static void setbit(Value& target, size_t idx, bool value);
};

