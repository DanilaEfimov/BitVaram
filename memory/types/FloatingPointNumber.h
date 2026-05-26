#pragma once

#include "SignedNumber.h"

#include <concepts>

template<typename T>
requires std::is_floating_point_v<T>
class FloatingPointNumber : public SignedNumber<T>
{
public:
	virtual int exponent() const = 0;
};
