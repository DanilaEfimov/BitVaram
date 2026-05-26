#pragma once

#include "Value.h"
#include "Bytes.h"
#include "Memory.h"

inline static constexpr size_t rawBytesSize = 16;

class RawBytes : public Value<Bytes<rawBytesSize>>
{
	using value_t = Bytes<rawBytesSize>;

	value_t data;

public:
	RawBytes() = default;

	value_t get() const override;
	void set(const value_t& val) override;
	void set(size_t pos, Memory::byte_t val);

	bool operator==(const value_t& right) const override;
	Memory::byte_t operator[](size_t pos) const;
};

