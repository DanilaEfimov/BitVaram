#include "Value.h"
#include "BitLine.h"

Value::Value(bool bit, size_t idx)
	: data(), type(bv::Type::Bit)
{
	BitLine::write(bit, idx);
}

Value::Value(size_t size)
	: data(size), type(bv::Bytes)
{
	if (size == 1)
		this->type = bv::Type::Byte;
}

Value::Value(Block&& data, bv::Type t)
	: data(std::move(data)), type(t)
{}

std::span<const byte_t> Value::bytes() const noexcept
{
	return this->data.bytes;
}

std::span<byte_t> Value::bytes() noexcept
{
	return this->data.bytes;
}

bv::Type Value::getType() const noexcept
{
	return this->type;
}
