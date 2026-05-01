#pragma once

#include <cstddef>
#include <vector>

using byte_t = std::byte;

class Programm
{
	std::vector<byte_t> code;

public:
	Programm(std::vector<byte_t> code);

	byte_t read(size_t idx) const;
	size_t size() const noexcept;
};

