#include "Uint32.h"

Uint32::Uint32(uint32_t num)
	: val(num)
{}

Uint32::Uint32()
	: val(0)
{}

uint32_t Uint32::get() const
{
	return this->val;
}

void Uint32::set(const uint32_t& val)
{
	this->val = val;
}

uint32_t Uint32::operator+(const Number<uint32_t>& right) const
{
	return this->val + right.get();
}

uint32_t Uint32::operator-(const Number<uint32_t>& right) const
{
	return this->val - right.get();
}

uint32_t Uint32::operator*(const Number<uint32_t>& right) const
{
	return this->val * right.get();
}

uint32_t Uint32::operator/(const Number<uint32_t>& right) const
{
	return this->val / right.get();
}

bool Uint32::operator>(const Number<uint32_t>& right) const
{
	return this->val > right.get();
}

bool Uint32::operator==(const uint32_t& right) const
{
	return this->val == right;
}
