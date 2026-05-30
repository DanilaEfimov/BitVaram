#pragma once

#include "Number.h"
#include "NumberExtensionConcept.h"

#include <concepts>

class BigIntImpl;

template<typename T>
requires std::is_signed_v<T> || NumberExtension<T>
class SignedNumber : public Number<T>
{
public:
	virtual int sign() const = 0;
	virtual T abs() const = 0;
};
