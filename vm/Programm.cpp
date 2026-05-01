#include "Programm.h"

Programm::Programm(std::vector<byte_t> code)
	: code(std::move(code))
{}

byte_t Programm::read(size_t idx) const
{
	return this->code.at(idx);
}

size_t Programm::size() const noexcept
{
	return this->code.size();
}

