#pragma once

#include <cstddef>
#include <vector>

using byte_t = std::byte;

struct Block
{
    std::vector<byte_t> bytes;

    Block() = default;
    explicit Block(size_t size);

    Block(const void* data, size_t size);
};

