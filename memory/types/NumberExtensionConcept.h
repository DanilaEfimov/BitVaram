#pragma once

#include <concepts>

class BigIntStorage;

template<typename T>
concept NumberExtension =
std::is_same_v<T, BigIntStorage>;
