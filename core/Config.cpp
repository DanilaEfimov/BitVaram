#include "Config.h"
#include "BitLine.h"

size_t Config::bitStorageSize = Config::bitChunkSize * 16;

size_t Config::chunkStorageSize = Config::bitStorageSize / Config::bitChunkSize;

size_t Config::bitChunkSize = sizeof(BitLine::chunk) * 8;

size_t Config::callStackSize = 128;
