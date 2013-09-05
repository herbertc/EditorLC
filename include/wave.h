#ifndef WAVE_H
#define WAVE_H

#include <SDL/SDL.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <string>
#include <vector>

#include "chunk.h"

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
	
private:
	vector<Chunk *> m_subchunks;
};

typedef struct _Wave_Header
{
  char chunkID[4];	//"RIFF"
  Uint32 chunkSize; 	//tamanho_do_arquivo-8
  char format[4];	//"WAVE"

}Wave_Header;

typedef struct _Subchunk1
{
  char subchunk1ID[4];		//"fmt "
  Uint32 subchunk1Size;		//16 - tamanho do bloco "fmt " (16 bytes)
  Uint16 audioFormat;		//1 - PCM
  Uint16 numChannels;		//1 = mono, 2 = stereo
  Uint32 sampleRate;		//amostras por segundo: 11025, 22050, 44100
  Uint32 byteRate;		//bytes por segundo: sampleRate * blockAlign ou 
  Uint16 blockAlign;		//numChannels*bitsperSample/8
  Uint16 bitsperSample;		//8 ou 16
  
}Subchunk1;

typedef struct _Subchunk2
{
  char subchunk2ID[4];		//"data"
  Uint32 subchunk2Size;		//tamanho do bloco "data": NumSamples * NumChannels * BitsPerSample/8
  char* data;			//amostra de dados - possui o número de bytes de subchunk2Size.
} Subchunk2;
  
typedef struct _subchunk
{
  char id[4];
  Uint32 size;
  unsigned char* data;
} subchunk;

typedef struct _wave_fmt
{
  Uint16 audioFormat;		//(2) 1 - PCM
  Uint16 numChannels;		//(2) 1 = mono, 2 = stereo
  Uint32 sampleRate;		//(4) amostras por segundo: 11025, 22050, 44100
  Uint32 byteRate;		//(4) bytes por segundo: sampleRate * blockAlign ou 
  Uint16 blockAlign;		//(2) numChannels*bitsperSample/8
  Uint16 bitsperSample;		//(2) 8 ou 16
  
}wave_fmt;
  
extern Wave_Header* create_wave_header();

extern wave_fmt* create_wave_fmt();

extern subchunk* create_subchunk();

//extern Subchunk1* create_subchunk1();

//extern Subchunk2* create_subchunk2();

extern int load_subchunk(subchunk *sc, const char * id, FILE *F);

extern void load_wav_header(Wave_Header* wh, FILE *F);

//extern void load_subchunk1(Subchunk1* sc1, FILE *F);

//extern void load_subchunk2(Subchunk2* sc2, FILE *F);

extern long wave_size(const char* path);

extern int parse_fmt_data(wave_fmt* fmt, const subchunk* sc);

extern void* parse_wave_data(subchunk *sc);

extern void print_header_file(Wave_Header *wh);

//extern void print_subchunk1(Subchunk1 *sc1);

//extern void print_subchunk2(Subchunk2 *sc2);

extern SDL_AudioSpec* Load_Wave(const char *file, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len);

#endif

