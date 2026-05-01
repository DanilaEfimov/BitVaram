#include "Block.h"

Block::Block(size_t size)
	: bytes(size, static_cast<byte_t>(0))
{}

Block::Block(const void* data, size_t size)
	: bytes(static_cast<const byte_t*>(data),
		static_cast<const byte_t*>(data) + size)
{}
