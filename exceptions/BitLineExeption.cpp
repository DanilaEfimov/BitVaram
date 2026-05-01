#include "BitLineExeption.h"

BitLineExeption::BitLineExeption(const char* msg)
	: std::exception(msg)
{}
