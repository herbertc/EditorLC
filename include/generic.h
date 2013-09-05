#ifndef GENERIC_H
#define GENERIC_H

#include <iostream>
#include "chunk.h"

using namespace std;

class Generic : public Chunk {
public:
	Generic();
	~Generic();

	Data * encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);
	
	void print(ostream& os) const;

	string id() const;
	uint32_t size() const;
	const uint8_t * data() const;

private:
	string m_id;
	uint32_t m_size;
	uint8_t *m_data;
};

#endif
