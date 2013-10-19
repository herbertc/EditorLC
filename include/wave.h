#ifndef WAVE_H
#define WAVE_H

#include <SDL/SDL.h>
#include <string>
#include <vector>

#include "chunk.h"
#include "format.h"
#include "generic.h"

using namespace std;

class Wave : public Chunk {
public:	
	~Wave();

	Data * encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);

	void print(ostream& os) const;
	void save(const string& path) const;

	static Wave * load(const string& path);
	static const string id;
	static const uint32_t size;

	void add_chunk(Chunk *chunk);
	
	SDL_AudioSpec spec() const;
	Generic *data() const;

private:
	vector<Chunk *> m_subchunks;

	Format *format() const;

};

extern SDL_AudioSpec* Load_Wave(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len);

#endif

