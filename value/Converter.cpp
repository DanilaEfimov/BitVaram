#include "Converter.h"

Value Converter::_int_from_bit(bool val) noexcept
{
    return Value();
}

Value Converter::_float_from_bit(bool val) noexcept
{
    return Value();
}

Value Converter::_int_from_float(const Value& val) noexcept
{
    return Value();
}

Value Converter::_float_from_int(const Value& val) noexcept
{
    return Value();
}

Value Converter::int_cast(const Value& val)
{
    switch (val.getType()) {
    case bv::Type::Byte: break;
    default:
        return val;
    }
}

Value Converter::float_cast(const Value& val) noexcept
{
    return Value();
}

void Converter::bool_cast(const Value& val, size_t& idx) noexcept
{
}
