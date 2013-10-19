#ifndef FORMAT_H
#define FORMAT_H

#include <stdint.h>
#include <string.h>

#include "chunk.h"

using namespace std;

class Format : public Chunk
{
public:
	Format();
	
	Data * encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);
	
	uint16_t audioFormat() const;
	uint16_t numChannels() const;
	uint32_t sampleRate() const;
	uint16_t bitsPerSample() const;
	
	void print(ostream& os) const;
	
	static const string id;

private:
	uint16_t m_audioFormat;
	uint16_t m_numChannels;
	uint32_t m_sampleRate;
	uint32_t m_byteRate; 
	uint16_t m_blockAlign;
	uint16_t m_bitsperSample;
	
	uint32_t size() const;
	
};

#endif
