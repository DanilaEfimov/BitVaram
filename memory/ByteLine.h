#pragma once

#include "Memory.h"

#include <vector>

class ByteLine : public Memory
{
	std::vector<Memory::byte_t> line;

public:
	ByteLine(size_t capacity = 256);

	size_t size() const noexcept override;

	byte_t read(size_t idx) const override;
	void write(size_t idx, byte_t value) override;

	size_t readBytes(size_t offset, byte_t* buffer, size_t count) const override;
	size_t writeBytes(size_t offset, const byte_t* buffer, size_t count) override;

	const std::vector<byte_t>& data() const noexcept;
};

