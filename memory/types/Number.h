#pragma once

#include "Value.h"
#include "NumberExtensionConcept.h"

#include <concepts>

class BigIntImpl;

template<typename T>
requires std::is_arithmetic_v<T> || NumberExtension<T>
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
		return !this->operator==(right.get()) && !this->operator>(right);
	}
};
