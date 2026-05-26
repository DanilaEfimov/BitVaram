#pragma once

#include "Value.h"

#include <vector>
#include <stdexcept>

template<typename T>
class Array : public Value<T*>
{
	std::vector<T> elements;

public:
	[[deprecated("use operator[] instead")]] virtual T* get() const
	{
		return this->elements.data();
	}

	[[deprecated("use operator[] instead")]] virtual void set(const T*& val)
	{
		if (this->elements.data() == val)
			return;

		this->elements.clear();
		this->elements.data() = val;
	}

	[[deprecated("Value legacy api")]] virtual bool operator==(const T*& right) const
	{
		return this->elements.data() == right;
	}

	T operator[](size_t pos) const
	{
		if (pos >= this->size())
			throw std::out_of_range("Array::operator[]: index out of range");

		return this->elements.at(pos);
	}

	size_t size() const noexcept
	{
		return this->elements.size();
	}

	bool empty() const noexcept
	{
		return this->elements.empty();
	}

	void append(const T& value)
	{
		this->elements.push_back(value);
	}

	void remove(size_t idx)
	{
		if (idx >= this->size())
			throw std::out_of_range("Array::operator[]: index out of range");

		this->elements.erase(this->elements.begin() + idx);
	}

	void clear()
	{
		this->elements.clear();
	}

	void reserve(size_t size)
	{
		this->elements.reserve(size);
	}
};

