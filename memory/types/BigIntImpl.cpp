#include "BigIntImpl.h"

void BigIntImpl::add(BigIntStorage& left, const BigIntStorage& right)
{
    
}

BigIntStorage BigIntImpl::add(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::subtract(BigIntStorage& left, const BigIntStorage& right)
{
}

BigIntStorage BigIntImpl::subtract(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::multiply(BigIntStorage& left, const BigIntStorage& right)
{
}

BigIntStorage BigIntImpl::multiply(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::divide(BigIntStorage& left, const BigIntStorage& right)
{
}

BigIntStorage BigIntImpl::divide(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::modulo(BigIntStorage& left, const BigIntStorage& right)
{
}

BigIntStorage BigIntImpl::modulo(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::abs(BigIntStorage& left)
{
}

BigIntStorage BigIntImpl::abs(const BigIntStorage& left)
{
}

void BigIntImpl::bitAnd(BigIntStorage& left, const BigIntStorage& right)
{
}

BigIntStorage BigIntImpl::bitAnd(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::bitOr(BigIntStorage& left, const BigIntStorage& right)
{
}

BigIntStorage BigIntImpl::bitOr(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::bitXor(BigIntStorage& left, const BigIntStorage& right)
{
}

BigIntStorage BigIntImpl::bitXor(const BigIntStorage& left, const BigIntStorage& right)
{
    return {};
}

void BigIntImpl::shiftLeft(BigIntStorage& value, std::size_t shift)
{
}

BigIntStorage BigIntImpl::shiftLeft(const BigIntStorage& value, std::size_t shift)
{
    return {};
}

void BigIntImpl::shiftRight(BigIntStorage& value, std::size_t shift)
{
}

BigIntStorage BigIntImpl::shiftRight(const BigIntStorage& value, std::size_t shift)
{
    return {};
}

int BigIntImpl::compare(const BigIntStorage& left, const BigIntStorage& right)
{
    return 0;
}

int BigIntImpl::sign(const BigIntStorage& left)
{
    bool zero = true;
    for (size_t i = 0; i < left.size(); i++) {
        if (left.read(i)) {
            zero = false;
            break;
        }
    }
    if (zero)
        return 0;

    return left.read(0) ? -1 : 1;
}
