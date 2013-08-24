#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include <string>

using namespace std;

class Chunk {
public:
	Chunk();
	~Chunk();

	void read(uint8_t *data);
	int write(uint8_t *data);

	string id() const;
	uint32_t size() const;
	uint8_t * data() const;
	
	void setID(const string& id);
	void setData(const uint8_t *data, uint32_t size);

private:
	char _id[4];
	uint32_t _size;
	uint8_t *_data;

	bool realloc(uint32_t size);
};

#endif

