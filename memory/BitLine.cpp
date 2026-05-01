#include "BitLine.h"
#include "Config.h"
#include "exceptions/ExOutOfBitLine.h"

BitLine::chunk* BitLine::storage = new chunk[Config::chunkStorageSize];

void BitLine::setbit(chunk& ch, size_t idx) noexcept
{
    auto mask = chunk(1) << (idx % Config::bitChunkSize);
    ch |= mask;
}

bool BitLine::getbit(chunk& ch, size_t idx) noexcept
{
    auto mask = chunk(1) << (idx % Config::bitChunkSize);
    return ch & mask ? true : false;
}

void BitLine::resetbit(chunk& ch, size_t idx) noexcept
{
    auto mask = chunk(1) << (idx % Config::bitChunkSize);
    ch &= ~mask;
}

BitLine::chunk& BitLine::getchunk(size_t idx)
{
    size_t chunkIdx = idx / Config::bitChunkSize;
    
    if (chunkIdx >= Config::chunkStorageSize)
        throw ExOutOfBitLine("BitLine::getChunk::chunk out of bitline");

    return BitLine::storage[chunkIdx];
}

bool BitLine::read(size_t idx) 
{
    size_t chunkIdx = idx / Config::bitChunkSize;
    size_t bitIdx = idx % Config::bitChunkSize;

    uint64_t& chunkRef = BitLine::getchunk(chunkIdx);
    return BitLine::getbit(chunkRef, bitIdx);
}

void BitLine::write(bool val, size_t idx)
{
    size_t chunkIdx = idx / Config::bitChunkSize;
    size_t bitIdx = idx % Config::bitChunkSize;

    uint64_t& chunkRef = BitLine::getchunk(chunkIdx);

    if (val)
        BitLine::setbit(chunkRef, bitIdx);
    else
        BitLine::resetbit(chunkRef, bitIdx);
}
