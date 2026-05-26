#pragma once

#include "Number.h"

#include <concepts>

template<typename T>
requires std::is_signed_v<T>
class SignedNumber : public Number<T>
{
public:
	virtual int sign() const = 0;
	virtual T abs() const = 0;
};
