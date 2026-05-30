#pragma once

#include "SignedNumber.h"
#include "BigIntStorage.h"

struct BigIntImpl
{
	enum CompareResult : int {
		eq = 0,
		less = -1,
		greater = 1
	};

	static void add(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage add(const BigIntStorage& left, const BigIntStorage& right);

	static void subtract(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage subtract(const BigIntStorage& left, const BigIntStorage& right);

	static void multiply(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage multiply(const BigIntStorage& left, const BigIntStorage& right);

	static void divide(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage divide(const BigIntStorage& left, const BigIntStorage& right);

	static void modulo(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage modulo(const BigIntStorage& left, const BigIntStorage& right);

	static void abs(BigIntStorage& left);
	static BigIntStorage abs(const BigIntStorage& left);

	static void bitAnd(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage bitAnd(const BigIntStorage& left, const BigIntStorage& right);

	static void bitOr(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage bitOr(const BigIntStorage& left, const BigIntStorage& right);

	static void bitXor(BigIntStorage& left, const BigIntStorage& right);
	static BigIntStorage bitXor(const BigIntStorage& left, const BigIntStorage& right);

	static void shiftLeft(BigIntStorage& value, std::size_t shift);
	static BigIntStorage shiftLeft(const BigIntStorage& value, std::size_t shift);

	static void shiftRight(BigIntStorage& value, std::size_t shift);
	static BigIntStorage shiftRight(const BigIntStorage& value, std::size_t shift);

	static int compare(const BigIntStorage& left, const BigIntStorage& right);
	static int sign(const BigIntStorage& left);
};

