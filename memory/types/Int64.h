#pragma once

#include "SignedNumber.h"

class Int64 final : public SignedNumber<int64_t>
{
	int64_t val;

public:
	Int64(int64_t val);
	Int64();

	int64_t get() const override;
	void set(const int64_t& val) override;

	int64_t operator+(const Number<int64_t>& right) const override;
	int64_t operator-(const Number<int64_t>& right) const override;
	int64_t operator*(const Number<int64_t>& right) const override;
	int64_t operator/(const Number<int64_t>& right) const override;

	bool operator>(const Number<int64_t>& right) const override;
	bool operator==(const int64_t& right) const override;

	int sign() const override;
	int64_t abs() const override;
};

