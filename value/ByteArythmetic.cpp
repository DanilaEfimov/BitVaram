#include "ByteArythmetic.h"
#include "Config.h"

bool ByteArythmetic::getbit(const Value& val, size_t idx)
{
    size_t byteidx = idx / 8;
    size_t bitidx = idx % 8;

    byte_t byteval = val.bytes().data()[byteidx];

    return static_cast<unsigned char>(byteval) & (1 << bitidx);
}

void ByteArythmetic::setbit(Value& target, size_t idx, bool value)
{
    size_t byteidx = idx / 8;
    size_t bitidx = idx % 8;

    auto* data = target.bytes().data();

    uint8_t byteval = std::to_integer<uint8_t>(data[byteidx]);

    if (value)
        byteval |= (1u << bitidx);      
    else
        byteval &= ~(1u << bitidx);     

    data[byteidx] = byte_t{ byteval };
}
