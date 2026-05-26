#pragma once

class Memory
{
public:
	using byte_t = unsigned char;

	virtual ~Memory() = default;

	virtual size_t size() const noexcept = 0;
	virtual byte_t read(size_t idx) const = 0;
	virtual void write(size_t idx, byte_t value) = 0;

	virtual size_t readBytes(size_t offset, byte_t* buffer, size_t count) const = 0;
	virtual size_t writeBytes(size_t offset, const byte_t* buffer, size_t count) = 0;
};

