#pragma once

#include "Value.h"
#include "ByteLine.h"

class Bit : public Value<bool>
{
	static size_t current;
	static ByteLine line;

public:
	Bit(size_t pos, bool val);

	static void write(size_t pos, bool value);
	static bool read(size_t pos);

	bool get() const override;
	void set(const bool& val) override;

	bool operator==(const bool& right) const override;

	static void setByteLineSize(size_t size);
};

