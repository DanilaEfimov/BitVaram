#pragma once

#include <cstdint>

namespace bv {

	enum Type : uint8_t
	{
		Int,
		Float,
		Bytes,
		Byte,
		Bit
	};

	inline constexpr bool arythmeticType(Type t) noexcept {
		return t == Int || t == Float;
	}

}