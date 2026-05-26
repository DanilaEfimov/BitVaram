#pragma once

#include "SignedNumber.h"

class Int32 : public SignedNumber<int32_t>
{
	int32_t val;

public:
	explicit Int32(int32_t num);
	explicit Int32();

	int32_t get() const override;
	void set(const int32_t& val) override;

	int32_t operator+(const Number<int32_t>& right) const override;
	int32_t operator-(const Number<int32_t>& right) const override;
	int32_t operator*(const Number<int32_t>& right) const override;
	int32_t operator/(const Number<int32_t>& right) const override;
	
	bool operator>(const Number<int32_t>& right) const override;
	bool operator==(const Value<int32_t>& right) const override;

	int sign() const override;
	int32_t abs() const override;
};

