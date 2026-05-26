#pragma once

#include "Number.h"
#include "Bytes.h"

#include <concepts>

template<typename T>
requires std::is_unsigned_v<T>
class UnsignedNumber : Number<T>
{
	T num;

public:
	int sign() const noexcept
	{
		if (this->num == 0)
			return 0;

		return this->num > 0 ? 1 : -1;
	}

	T abs() const
	{
		return std::abs(this->num);
	}
};
