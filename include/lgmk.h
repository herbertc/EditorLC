#ifndef LGMK_H
#define LGMK_H

#include <string.h>

#include <stdint.h>

#include <algorithm>
#include <vector>
#include <string>

#include "chunk.h"

class Lgmk : public Chunk
{
public:
	Lgmk();
	
	Data *encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);
	
	void print(ostream& os) const;
	
	vector<uint32_t> marks() const;
	
	void add_mark(uint32_t position);
	
	static const string id;
private:
	vector<uint32_t> m_marks; 

	uint32_t size() const;
};

#endif
