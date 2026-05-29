#pragma once

#include "Value.h"
#include "BigIntStorage.h"

class BigInt : public Value<BigIntStorage>
{
	BigIntStorage storage;

public:
	BigIntStorage get() const override;
	void set(const BigIntStorage& val) override;

	bool operator==(const BigIntStorage& right) const override;
};

