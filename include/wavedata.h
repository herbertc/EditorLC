#ifndef WAVEDATA_H
#define WAVEDATA_H

#include <string>

#include "chunk.h"
#include "data.h"

class WaveData : public Chunk
{
public:
	WaveData();
	
	static const string id;
	
	Data *encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);
	
	void print(ostream& os) const;
	
	uint32_t size() const;
private:
	//uint32_t m_size;
	string m_data;
};

#endif