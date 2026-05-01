#include "StackException.h"

StackException::StackException(const char* msg)
	: std::exception(msg)
{}
