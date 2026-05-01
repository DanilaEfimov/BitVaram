#pragma once

#include "Config.h"

#include <cstddef>

namespace {
	struct addcarry_tag {};
	struct subborrow_tag {};

	template<typename T>
	constexpr bool always_false = false;
}

template<typename Op>
void apply(byte_t* left, byte_t* right) {
	static_assert(always_false<Op>, "requires specialization");
};

template<>
void apply<addcarry_tag>(byte_t* left, byte_t* right) {
	unsigned char carry = 0;
	for (size_t i = 0; i < Config::intSize; i++) {
		carry = _addcarry_u8(
			carry,
			(unsigned char)left[i],
			(unsigned char)right[i],
			(unsigned char*)&left[i]
		);
	}
};

template<>
void apply<subborrow_tag>(byte_t* left, byte_t* right) {
	unsigned char carry = 0;
	for (size_t i = 0; i < Config::intSize; i++) {
		carry = _subborrow_u8(
			carry,
			(unsigned char)left[i],
			(unsigned char)right[i],
			(unsigned char*)&left[i]
		);
	}
};
