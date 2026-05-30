#include "BigInt.h"
#include "BigIntImpl.h"

BigInt::BigInt()
	: SignedNumber<BigIntStorage>(),
	bytes()
{}

BigInt::BigInt(const BigIntStorage& storage)
	: SignedNumber<BigIntStorage>(),
	bytes(storage)
{}

BigIntStorage BigInt::get() const
{
	return this->bytes;
}

void BigInt::set(const BigIntStorage& val)
{
	this->bytes = val;
}

bool BigInt::operator==(const BigIntStorage& right) const
{
	return BigIntImpl::compare(this->bytes, right) == BigIntImpl::CompareResult::eq;
}

BigIntStorage BigInt::operator+(const Number<BigIntStorage>& right) const
{
	return BigIntImpl::add(this->bytes, right.get());
}

BigIntStorage BigInt::operator-(const Number<BigIntStorage>& right) const
{
	return BigIntImpl::subtract(this->bytes, right.get());
}

BigIntStorage BigInt::operator*(const Number<BigIntStorage>& right) const
{
	return BigIntImpl::multiply(this->bytes, right.get());
}

BigIntStorage BigInt::operator/(const Number<BigIntStorage>& right) const
{
	return BigIntImpl::divide(this->bytes, right.get());
}

bool BigInt::operator>(const Number<BigIntStorage>& right) const
{
	return BigIntImpl::compare(this->bytes, right.get()) == BigIntImpl::CompareResult::less;
}

int BigInt::sign() const
{
	return BigIntImpl::sign(this->bytes);
}

BigIntStorage BigInt::abs() const
{
	return BigIntImpl::abs(this->bytes);
}
