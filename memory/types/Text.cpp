#include "Text.h"

Text::Text(const std::string& str)
    : text(str)
{}

std::string Text::get() const
{
    return this->text;
}

void Text::set(const std::string& val)
{
    this->text = val;
}

bool Text::operator==(const std::string& right) const
{
    return this->text == right;
}

Text Text::operator+(const Text& right) const
{
    return Text(this->text + right.get());
}
