#include "BigInt.h"

BigIntStorage BigInt::get() const
{
    return this->storage;
}

void BigInt::set(const BigIntStorage& val)
{
    this->storage = val;
}

bool BigInt::operator==(const BigIntStorage& right) const
{
    return this->storage == right;
}
