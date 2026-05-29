#include "Double.h"

#include <cmath>

Double::Double(double val)
	: num(val)
{}

Double::Double()
	: num(0.0)
{}

double Double::get() const
{
	return this->num;
}

void Double::set(const double& val)
{
	this->num = val;
}

double Double::operator+(const Number<double>& right) const
{
	return this->num + right.get();
}

double Double::operator-(const Number<double>& right) const
{
	return this->num - right.get();
}

double Double::operator*(const Number<double>& right) const
{
	return this->num * right.get();
}

double Double::operator/(const Number<double>& right) const
{
	return this->num / right.get();
}

bool Double::operator>(const Number<double>& right) const
{
	return this->num > right.get();
}

bool Double::operator==(const double& right) const
{
	return this->num == right;
}

int Double::sign() const
{
	if (this->num == 0.0)
		return 0;

	return this->num > 0.0 ? 1 : -1;
}

double Double::abs() const
{
	return std::abs(this->num);
}

int Double::exponent() const
{
	return std::ilogb(this->num);
}
