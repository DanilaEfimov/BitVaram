#include "TypeException.h"

TypeException::TypeException(const char* msg)
	: std::exception(msg)
{}
