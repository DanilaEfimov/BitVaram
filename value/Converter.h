#pragma once

#include "types.h"
#include "Value.h"

class Converter
{
	static Value _int_from_bit(bool val) noexcept;

	static Value _float_from_bit(bool val) noexcept;

	static Value _int_from_float(const Value& val) noexcept;

	static Value _float_from_int(const Value& val) noexcept;

public:
	static Value int_cast(const Value& val);

	static Value float_cast(const Value& val) noexcept;

	static void bool_cast(const Value& val, size_t& idx) noexcept;
};

