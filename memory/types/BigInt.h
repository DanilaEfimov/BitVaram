#pragma once

#include "SignedNumber.h"
#include "BigIntStorage.h"

class BigInt : public SignedNumber<BigIntStorage>
{
	BigIntStorage bytes;

public:
	BigInt();
	BigInt(const BigIntStorage& storage);

	BigIntStorage get() const override;
	void set(const BigIntStorage& val) override;
	bool operator==(const BigIntStorage& right) const override;
	
	BigIntStorage operator+(const Number<BigIntStorage>& right) const override;
	BigIntStorage operator-(const Number<BigIntStorage>& right) const override;
	BigIntStorage operator*(const Number<BigIntStorage>& right) const override;
	BigIntStorage operator/(const Number<BigIntStorage>& right) const override;

	bool operator>(const Number<BigIntStorage>& right) const override;
	
	int sign() const override;
	BigIntStorage abs() const override;
};

