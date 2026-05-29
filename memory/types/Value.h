#pragma once

#include "IValue.h"

template<typename T>
class Value : public IValue
{
public:
	Value() = default;
	virtual ~Value() = default;

	virtual T get() const = 0;
	virtual void set(const T& val) = 0;

	virtual bool operator==(const T& right) const = 0;
};

