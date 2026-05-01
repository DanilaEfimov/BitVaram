#pragma once

#include <stdexcept>

class BitLineExeption : public std::exception
{
public:
	BitLineExeption(const char* msg);
};

