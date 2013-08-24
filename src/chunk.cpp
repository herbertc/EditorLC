#include <cstring>
#include "chunk.h"
#include <stdio.h>

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
Chunk::read(uint8_t *data)
{
	memcpy(_id, data, 4);

	int size;
	memcpy(&size, data + 4, 4);
	
	if (size > _size)
		realloc(size);

	memcpy(_data, data + 8, size);
}

int 
Chunk::write(uint8_t *data)
{
	return 0;
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
	if (_data > 0)
	{
		delete [] _data;
	}

	_data = new uint8_t[size];

	if (_data > 0)
	{
		_size = size;
	}

	return _data > 0;
}

