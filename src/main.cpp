#include "wave.h"
#include "chunk.h"
#include <stdint.h>
#include <string.h>

void teste(FILE *wave);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s {wave_file.wav}\n", argv[0]);
		return -1;
	}
	
	char *path = argv[1];

	FILE *wave_file = fopen(path, "rb");
	
	if(wave_file == NULL)
	  return -1;

	teste(wave_file);

/*	
	//Wave_Header* wave_header = create_wave_header();
	wave_fmt* fmt = create_wave_fmt();
	subchunk* sc = create_subchunk();
	Uint8* data;
	
	//load_wav_header(wave_header, wave_file);
	load_subchunk(sc, "fmt " , wave_file);
	
	///cout<< "id = " << sc->id << ", size = "<< sc->size << endl;
	parse_fmt_data(fmt, sc->data);
	//cout << "NumSamples :" << (8*subchunk2->subchunk2Size)/(subchunk1->numChannels*subchunk1->bitsperSample) << endl;
 
	load_subchunk(sc, "data" , wave_file);
	data = (Uint8*) parse_wave_data(sc);
	
	//cout<< "id = " << sc->id << ", size = " << sc->size << endl;
*/
	
	fclose(wave_file);
	
	return 0;
}

void teste(FILE *wave)
{
	fseek(wave, 0, SEEK_END);
	long size = ftell(wave);
	fseek(wave, 0, SEEK_SET);

	uint8_t *bytes = new uint8_t[size];
	fread(bytes, sizeof(uint8_t), size, wave);

	printf("%c", bytes[0]);

	delete [] bytes;
}

