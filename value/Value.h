#pragma once

#include "Block.h"
#include "types.h"

#include <cstdint>
#include <vector>
#include <span>

struct Value
{
private:
	Block data;
	bv::Type type;

public:
	Value() = default;
	explicit Value(bool bit, size_t idx);
	explicit Value(size_t size);
	explicit Value(Block&& data, bv::Type t);

	std::span<const byte_t> bytes() const noexcept;
	std::span<byte_t> bytes() noexcept;

	bv::Type getType() const noexcept;
};

