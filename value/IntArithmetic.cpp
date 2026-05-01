#include "IntArithmetic.h"
#include "Arythmetic.h"
#include "ByteArythmetic.h"
#include "Config.h"
#include "exceptions/ExTypeMismatch.h"

#include <cmath>
#include <stdlib.h>

bool IntArythmetic::_add_sign(const Value& left, const Value& right) noexcept
{
	bool lsign = IntArythmetic::sign(left);
	bool rsign = IntArythmetic::sign(right);

	if (lsign == rsign)
		return lsign;

	bool littler = IntArythmetic::le_abs(left, right);

	return littler ^ lsign;
}

Value& IntArythmetic::add(Value& left, Value& right)
{
	if (left.getType() != bv::Type::Int || right.getType() != bv::Type::Int)
		throw ExTypeMismatch("IntArithmetic::add::type mismatch");

	auto lbytes = left.bytes().data();
	auto rbytes = right.bytes().data();
	bool sign = IntArythmetic::_add_sign(left, right);

	if (IntArythmetic::sign(left) != IntArythmetic::sign(right))
		apply<subborrow_tag>(lbytes, rbytes);
	else
		apply<addcarry_tag>(lbytes, rbytes);

	IntArythmetic::setsign(left, sign);
	return left;
}

bool IntArythmetic::sign(const Value& val)
{
	if (val.getType() != bv::Type::Int)
		throw ExTypeMismatch("IntArythmetic::sign::type mismatch");

	return ByteArythmetic::getbit(val, ByteArythmetic::signbit);
}

Value IntArythmetic::abs(const Value& val)
{
	Value ret = val;
	IntArythmetic::abs(val);
	return ret;
}

Value& IntArythmetic::abs(Value& val)
{
	IntArythmetic::positive(val);
	return val;
}

void IntArythmetic::negative(Value& val)
{
	if (val.getType() != bv::Type::Int)
		throw ExTypeMismatch("IntArythmetic::negative::type mismatch");

	ByteArythmetic::setbit(val, ByteArythmetic::signbit, true);
}

void IntArythmetic::positive(Value& val)
{
	if (val.getType() != bv::Type::Int)
		throw ExTypeMismatch("IntArythmetic::positive::type mismatch");

	ByteArythmetic::setbit(val, ByteArythmetic::signbit, false);
}

void IntArythmetic::setsign(Value& val, bool sign)
{
	if (val.getType() != bv::Type::Int)
		throw ExTypeMismatch("IntArythmetic::setsign::type mismatch");

	ByteArythmetic::setbit(val, ByteArythmetic::signbit, sign);
}

bool IntArythmetic::eq(const Value& left, const Value& right)
{
	if (left.getType() != bv::Type::Int || right.getType() != bv::Type::Int)
		throw ExTypeMismatch("IntArithmetic::add::type mismatch");

	const auto& lbytes = left.bytes();
	const auto& rbytes = right.bytes();

	bool isNull = true;
	for (size_t i = 0; i < Config::intSize - 1; i++) {
		const byte_t& lbyte = lbytes[Config::intSize - i - 1];
		const byte_t& rbyte = rbytes[Config::intSize - i - 1];

		isNull &= static_cast<bool>(lbyte | rbyte) == 0x00;

		if (lbyte != rbyte)
			return false;
	}

	const auto& llast = lbytes[Config::intSize - 1];
	const auto& rlast = rbytes[Config::intSize - 1];

	if (isNull)
		return !(static_cast<unsigned char>(
				llast ^ rlast
				) & 0b01111111);

	return llast == rlast;
}

bool IntArythmetic::le(const Value& left, const Value& right)
{
	if (left.getType() != bv::Type::Int || right.getType() != bv::Type::Int)
		throw ExTypeMismatch("IntArithmetic::add::type mismatch");

	bool lsign = IntArythmetic::sign(left);
	bool rsign = IntArythmetic::sign(right);

	if (lsign != rsign)
		return lsign;

	const auto& lbytes = left.bytes();
	const auto& rbytes = right.bytes();

	for (size_t i = 0; i < Config::intSize; i++) {
		const byte_t& lbyte = lbytes[Config::intSize - i - 1];
		const byte_t& rbyte = rbytes[Config::intSize - i - 1];

		if (lbyte < rbyte) return !lsign;
		if (lbyte > rbyte) return lsign;
	}

	return true;
}

bool IntArythmetic::ge(const Value& left, const Value& right)
{
	return !IntArythmetic::le(left, right) || IntArythmetic::eq(left, right);
}

bool IntArythmetic::le_abs(const Value& left, const Value& right)
{
	bool lsign = IntArythmetic::sign(left);
	bool rsign = IntArythmetic::sign(right);
	
	if (lsign != rsign)
		return lsign >= rsign;

	if (lsign == true)
		return !IntArythmetic::ge(left, right);
	else
		return IntArythmetic::le(left, right);
}
