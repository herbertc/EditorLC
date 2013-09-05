#ifndef CHUNK_FACTORY_H
#define CHUNK_FACTORY_H

#include "chunk.h"

using namespace std;

class ChunkFactory {
public:
	static Chunk * decode(const Data& data, uint32_t *decoded, uint32_t offset = 0);
};

#endif
