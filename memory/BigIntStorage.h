#pragma once

#include "Memory.h"

#include <vector>

class BigIntStorage : public Memory
{
	std::vector<byte_t> bytes;

public:
	BigIntStorage();
	
	size_t size() const noexcept override;
	
	byte_t read(size_t idx) const override;
	void write(size_t idx, byte_t value) override;
	
	size_t readBytes(size_t offset, byte_t* buffer, size_t count) const override;
	size_t writeBytes(size_t offset, const byte_t* buffer, size_t count) override;

	bool operator==(const BigIntStorage& right) const;
};

