#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <string.h>
#include "data.h"

using namespace std;

class Chunk {
	friend ostream& operator<<(ostream& os, const Chunk *chunk)
	{
		chunk->print(os);
		return os;
	}

public:
	virtual ~Chunk() {}

	virtual Data * encode() const = 0;
	virtual uint32_t decode(const Data& data, uint32_t offset = 0) = 0;

	virtual void print(ostream& os) const = 0;
};

#endif
