#pragma once

#include "Value.h"

#include <string>

class Text : public Value<std::string>
{
	std::string text;

public:
	Text(const std::string& str);

	std::string get() const override;
	void set(const std::string& val) override;

	bool operator==(const std::string& right) const override;
	Text operator+(const Text& right) const;
};

