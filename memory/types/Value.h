#pragma once

template<typename T>
class Value
{
public:
	Value() = default;
	virtual ~Value() = default;

	virtual T get() const = 0;
	virtual void set(const T& val) = 0;

	virtual bool operator==(const Value<T>& right) const = 0;
};

