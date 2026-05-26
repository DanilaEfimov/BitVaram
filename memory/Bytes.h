#pragma once

#include "Memory.h"

#include <stdexcept>
#include <array>

template<size_t L>
class Bytes : public Memory
{
	std::array<Memory::byte_t, L> chunk;

public:
	Bytes() = default;
	
	size_t size() const noexcept override
	{
		return L;
	}

	byte_t read(size_t idx) const override
	{
		if (idx >= L)
			throw std::out_of_range("Bytes::read: index out of range");

		return this->chunk[idx];
	}

	void write(size_t idx, byte_t value) override
	{
		if (idx >= L)
			throw std::out_of_range("Bytes::write: index out of range");

		this->chunk[idx] = value;
	}

	size_t readBytes(size_t offset, byte_t* buffer, size_t count) const override
	{
		if (offset + count > L)
			return 0;

		size_t n = std::min(count, this->size() - offset);
		std::memcpy(buffer, this->chunk.data() + offset, n);
		return n;
	}

	size_t writeBytes(size_t offset, const byte_t* buffer, size_t count) override
	{
		if (offset + count > L)
			return 0;

		size_t n = std::min(count, this->size() - offset);
		std::memcpy(this->chunk.data() + offset, buffer, n);
		return n;
	}
};

