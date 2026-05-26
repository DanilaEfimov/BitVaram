#include "Bit.h"
#include "ByteLine.h"

size_t Bit::current = 0;
ByteLine Bit::line = ByteLine(256);

void Bit::write(size_t pos, bool value)
{
	size_t bytePos = pos / 8;
	size_t bitPos = pos % 8;

	Memory::byte_t dest = Bit::line.read(pos);
	Memory::byte_t mask = static_cast<Memory::byte_t>(1 << bitPos);

	if (value)
		Bit::line.write(bytePos, dest | mask);
	else
		Bit::line.write(bytePos, dest & ~mask);
}

bool Bit::read(size_t pos)
{
	size_t bytePos = pos / 8;
	size_t bitPos = pos % 8;

	Memory::byte_t dest = Bit::line.read(pos);
	Memory::byte_t mask = static_cast<Memory::byte_t>(1 << bitPos);

	return dest & mask;
}

Bit::Bit(size_t pos, bool val)
{
	Bit::write(pos, val);
}

bool Bit::get() const
{
	return Bit::read(Bit::current);
}

void Bit::set(const bool& val)
{
	Bit::write(Bit::current, val);
}

bool Bit::operator==(const bool& right) const
{
	return Bit::read(current) == right;
}

void Bit::setByteLineSize(size_t size)
{
	static bool called = false;

	if (!called)
		Bit::line.line.resize(size);

	called = true;
}
