#include "Int64.h"

Int64::Int64(int64_t val)
	: val(val)
{}

Int64::Int64()
	: val(0)
{}

int64_t Int64::get() const
{
	return this->val;
}

void Int64::set(const int64_t& val)
{
	this->val = val;
}

int64_t Int64::operator+(const Number<int64_t>& right) const
{
	return this->val + right.get();
}

int64_t Int64::operator-(const Number<int64_t>& right) const
{
	return this->val - right.get();
}

int64_t Int64::operator*(const Number<int64_t>& right) const
{
	return this->val * right.get();
}

int64_t Int64::operator/(const Number<int64_t>& right) const
{
	return this->val / right.get();
}

bool Int64::operator>(const Number<int64_t>& right) const
{
	return this->val > right.get();
}

bool Int64::operator==(const Value<int64_t>& right) const
{
	return this->val == right.get();
}

int Int64::sign() const
{
	return this->val != 0;
}

int64_t Int64::abs() const
{
	return this->val;
}
