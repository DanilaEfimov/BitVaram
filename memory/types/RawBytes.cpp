#include "RawBytes.h"

RawBytes::value_t RawBytes::get() const
{
	return this->data;
}

void RawBytes::set(const value_t& val)
{
	this->data = val;
}

void RawBytes::set(size_t pos, Memory::byte_t val)
{
	this->data.write(pos, val);
}

bool RawBytes::operator==(const value_t& right) const
{
	for (size_t i = 0; i < rawBytesSize; i++) {
		if (this->data.read(i) != right.read(i))
			return false;
	}

	return true;
}

Memory::byte_t RawBytes::operator[](size_t pos) const
{
	return this->data.read(pos);
}
