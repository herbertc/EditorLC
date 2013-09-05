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
  
private:
  uint16_t _audioFormat;
  uint16_t _numChannels;
  uint32_t _sampleRate;
  uint32_t _byteRate; 
  uint16_t _blockAlign;
  uint16_t _bitsperSample;
};

#endif