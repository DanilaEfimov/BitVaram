#pragma once

#include "types.h"
#include "Value.h"

class IntArythmetic
{
	static bool _add_sign(const Value& left, const Value& right) noexcept;

public:
	static Value& add(Value& left, Value& right);

	static bool sign(const Value& val);
	static Value abs(const Value& val);
	static Value& abs(Value& val);

	static void negative(Value& val);
	static void positive(Value& val);
	static void setsign(Value& val, bool sign);

	static bool eq(const Value& left, const Value& right);
	static bool le(const Value& left, const Value& right);
	static bool ge(const Value& left, const Value& right);
	static bool le_abs(const Value& left, const Value& right);
};

