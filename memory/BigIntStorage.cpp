#include "BigIntStorage.h"

#include <stdexcept>

BigIntStorage::BigIntStorage()
    : bytes(1, 0)
{}

size_t BigIntStorage::size() const noexcept
{
    return this->bytes.size();
}

Memory::byte_t BigIntStorage::read(size_t idx) const
{
    if (idx >= this->size())
        throw std::out_of_range("BigIntStorage::read: index out of range");

    return this->bytes.at(idx);
}

void BigIntStorage::write(size_t idx, byte_t value)
{
    if (idx >= this->size())
        throw std::out_of_range("BigIntStorage::write: index out of range");

    this->bytes[idx] = value;
}

size_t BigIntStorage::readBytes(size_t offset, byte_t* buffer, size_t count) const
{
    if (offset + count > this->size())
        return 0;

    size_t n = std::min(count, this->size() - offset);
    std::memcpy(buffer, this->bytes.data() + offset, n);
    return n;
}

size_t BigIntStorage::writeBytes(size_t offset, const byte_t* buffer, size_t count)
{
    if (offset + count > this->size())
        return 0;

    size_t n = std::min(count, this->size() - offset);
    std::memcpy(this->bytes.data() + offset, buffer, n);
    return n;
}

bool BigIntStorage::operator==(const BigIntStorage& right) const
{
    return this->bytes == right.bytes;
}
