#include "Int32.h"

#include <cmath>

Int32::Int32(int32_t num)
	: val(num)
{}

Int32::Int32()
	: val(0)
{}

int32_t Int32::get() const
{
	return this->val;
}

void Int32::set(const int32_t& val)
{
	this->val = val;
}

int32_t Int32::operator+(const Number<int32_t>& right) const
{
	return this->val + right.get();
}

int32_t Int32::operator-(const Number<int32_t>& right) const
{
	return this->val - right.get();
}

int32_t Int32::operator*(const Number<int32_t>& right) const
{
	return this->val * right.get();
}

int32_t Int32::operator/(const Number<int32_t>& right) const
{
	return this->val / right.get();
}

bool Int32::operator>(const Number<int32_t>& right) const
{
	return this->val > right.get();
}

bool Int32::operator==(const Value<int32_t>& right) const
{
	return this->val == right.get();
}

int Int32::sign() const
{
	if (this->val == 0)
		return 0;

	return this->val > 0 ? 1 : -1;
}

int32_t Int32::abs() const
{
	return std::abs(this->val);
}
