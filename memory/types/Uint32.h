#pragma once

#include "UnsignedNumber.h"

class Uint32 : public UnsignedNumber<uint32_t>
{
	uint32_t val;

public:
	Uint32(uint32_t num);
	Uint32();

	uint32_t get() const override;
	void set(const uint32_t& val) override;

	uint32_t operator+(const Number<uint32_t>& right) const override;
	uint32_t operator-(const Number<uint32_t>& right) const override;
	uint32_t operator*(const Number<uint32_t>& right) const override;
	uint32_t operator/(const Number<uint32_t>& right) const override;

	bool operator>(const Number<uint32_t>& right) const override;
	bool operator==(const uint32_t& right) const override;
};

