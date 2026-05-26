#include "ByteLine.h"

#include <stdexcept>

ByteLine::ByteLine(size_t capacity)
    : line(capacity)
{}

size_t ByteLine::size() const noexcept
{
    return this->line.size();
}

ByteLine::byte_t ByteLine::read(size_t idx) const
{
    if (idx >= this->size())
        throw std::out_of_range("ByteLine::read: index out of range");

    return this->line.at(idx);
}

void ByteLine::write(size_t idx, byte_t value)
{
    if (idx >= this->size())
        throw std::out_of_range("ByteLine::write: index out of range");

    this->line[idx] = value;
}

size_t ByteLine::readBytes(size_t offset, byte_t* buffer, size_t count) const
{
    if (offset + count > this->size())
        return 0;

    size_t n = std::min(count, this->size() - offset);
    std::memcpy(buffer, this->line.data() + offset, n);
    return n;
}

size_t ByteLine::writeBytes(size_t offset, const byte_t* buffer, size_t count)
{
    if (offset + count > this->size())
        return 0;

    size_t n = std::min(count, this->size() - offset);
    std::memcpy(this->line.data() + offset, buffer, n);
    return n;
}

const std::vector<ByteLine::byte_t>& ByteLine::data() const noexcept
{
    return this->line;
}
