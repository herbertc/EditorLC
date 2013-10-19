#include <cstring>
#include <string>
#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>

const int Chunk::HEADER_SIZE = 8;

Chunk::Chunk()
{
	memcpy(_id, "UNDF", 4);
	_size = 0;
	_data = 0;
}

Chunk::~Chunk()
{
	if (_data)
	{
		delete [] _data;
	}
}

void 
Chunk::read(uint8_t *data, int index)
{
	memcpy(_id, data + index, 4);
	index += 4;

	uint32_t size;
	memcpy(&size, data + index, 4);
	index += 4;
	
	if (size > _size)
		realloc(size);

	memcpy(_data, data + index, size);
}

int 
Chunk::write(uint8_t *data, int index)
{
	if (!data)
		return 0;
		
	memcpy(data + index, _id, 4);
	index += 4;

	memcpy(data + index, &_size, 4);
	index += 4;

	memcpy(data + index, _data, _size);

	return _size + HEADER_SIZE;
}

uint8_t * 
Chunk::data() const
{
	return _data;
}

string 
Chunk::id() const
{
	char idAsCString[5];
	
	memcpy(idAsCString, _id, 4);
	idAsCString[4] = 0;

	string idString = idAsCString;
	return idString;
}
	
uint32_t 
Chunk::size() const
{
	return _size;
}

void 
Chunk::setID(const string& id)
{
	memcpy(_id, id.c_str(), 4);
}

void 
Chunk::setData(const uint8_t *data, uint32_t size)
{
	if (_size < size)
	{
		if (realloc(size) == false)
		{
			return;
		}
	}

	memcpy(_data, data, size);
	_size = size;
}

bool
Chunk::realloc(uint32_t size)
{
	if (_data != 0)
	{
		delete [] _data;
	}

	_data = new uint8_t[size];

	if (_data != 0)
	{
		_size = size;
	}

	return _data != 0;
}

