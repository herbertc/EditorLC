#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

class Chunk {
public:
	Chunk();
	virtual ~Chunk();

	virtual void read(uint8_t *data, int index = 0);
	virtual int write(uint8_t *data, int index = 0);

	string id() const;
	uint32_t size() const;
	uint8_t * data() const;
	
	void setID(const string& id);
	void setData(const uint8_t *data, uint32_t size);

	static const int HEADER_SIZE;

protected:
	char _id[4];
	uint32_t _size;
	uint8_t *_data;

	bool realloc(uint32_t size);
};

#endif
