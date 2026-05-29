#pragma once

#include "FloatingPointNumber.h"

class Double final : public FloatingPointNumber<double>
{
	double num;

public:
	Double(double val);
	Double();

	double get() const override;
	void set(const double& val) override;

	double operator+(const Number<double>& right) const override;
	double operator-(const Number<double>& right) const override;
	double operator*(const Number<double>& right) const override;
	double operator/(const Number<double>& right) const override;
	
	bool operator>(const Number<double>& right) const override;
	bool operator==(const double& right) const override;

	int sign() const override;
	double abs() const override;
	int exponent() const override;
};

