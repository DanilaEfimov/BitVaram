#pragma once

#include "Value.h"

#include <concepts>

template<typename T>
requires std::is_arithmetic_v<T>
class Number : public Value<T>
{
public:
	virtual T operator+(const Number<T>& right) const = 0;
	virtual T operator-(const Number<T>& right) const = 0;
	virtual T operator*(const Number<T>& right) const = 0;
	virtual T operator/(const Number<T>& right) const = 0;

	virtual bool operator>(const Number<T>& right) const = 0;
	virtual bool operator<(const Number<T>& right) const
	{
		return !this->operator==(right) && !this->operator>(right);
	}
};
