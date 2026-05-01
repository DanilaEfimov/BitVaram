#pragma once

class Config
{
public:
	static size_t bitStorageSize;	// in bits
	static size_t chunkStorageSize;	// in chunks
	static size_t bitChunkSize;		// in bits

	static size_t callStackSize;	// pointers

	// Types
	static constexpr size_t intSize = 4;
};

