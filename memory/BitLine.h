#pragma once

#include <cstdint>
#include <vector>

class BitLine
{
public:
	using chunk = uint64_t;

private:
	static chunk* storage;

	static void setbit(chunk& ch, size_t idx) noexcept;
	static bool getbit(chunk& ch, size_t idx) noexcept;
	static void resetbit(chunk& ch, size_t idx) noexcept;
	static chunk& getchunk(size_t idx);

public:
	static bool read(size_t idx);
	static void write(bool val, size_t idx);
};

