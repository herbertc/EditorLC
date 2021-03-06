#ifndef FMT_H
#define FMT_H

#include <stdint.h>
#include <string.h>

#include "chunk.h"

using namespace std;

class Fmt : public Chunk
{
public:
	Fmt();
	
	Data * encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);
	
	uint16_t audioFormat() const;
	uint16_t numChannels() const;
	uint32_t sampleRate() const;
	
	void print(ostream& os) const;
	
	static const string id;
private:
	uint16_t _audioFormat;
	uint16_t _numChannels;
	uint32_t _sampleRate;
	uint32_t _byteRate; 
	uint16_t _blockAlign;
	uint16_t _bitsperSample;
	
	uint32_t size() const;
	
};

#endif