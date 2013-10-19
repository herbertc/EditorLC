#include <cstdio>
#include <fstream>
#include <string.h>
#include <string>
#include "chunkfactory.h"
#include "generic.h"
#include "wave.h" 
#include "format.h"
#include "data.h"

Wave * 
Wave::load(const string& path)
{
	// Open file
	ifstream file(path.c_str());
	
	if (file.is_open() == false)
		return 0;

	// Compute file size
	long begin = file.tellg();
	file.seekg(0, ios::end);
	long end = file.tellg();

	long file_size = end - begin;

	file.seekg(0, ios::beg);
	
	// Prepare byte array
	uint8_t *bytes = new uint8_t[file_size];

	if (bytes == 0)
	{
		file.close();
		return 0;
	}

	// Read file into byte array
	file.read((char *) bytes, file_size);
	file.close();

	// Prepara data to decode
	Data data;
	data.set(bytes, file_size);

	// Create a new wave
	Wave *wave = new Wave();

	if (wave == 0)
	{
		return 0;
	}

	// Decode bytes into wave
	if (wave->decode(data))
	{	
		return wave;
	} else
	{
		delete wave;
		return 0;
	}
}

const string Wave::id = "RIFF";

void 
Wave::print(ostream& os) const
{
	os << "[" << Wave::id << "]" << endl;

	int count = m_subchunks.size();

	for (int i = 0; i < count; i++)
		m_subchunks[i]->print(os);
}

Wave::~Wave()
{
	int count = m_subchunks.size();

	for (int i = 0; i < count; i++)
		delete m_subchunks[i];
}
		
uint32_t 
Wave::decode(const Data& data, uint32_t offset)
{
	const uint8_t *iterator = data.bytes(offset);
	uint32_t decoded = 0;
	
	// Check id
	if (memcmp(iterator, Wave::id.c_str(), 4))
		return 0;

	decoded += 4;

	// Read size
	uint32_t size;
	memcpy(&size, iterator + decoded, 4);

	decoded += 4;

	// Check wave format
	if (memcmp(iterator + decoded, "WAVE", 4))
		return 0;

	decoded += 4;

	// Read subchunks
	Chunk *chunk = 0;
	
	while (decoded + offset < data.size())
	{
		chunk = ChunkFactory::decode(data, &decoded, offset + decoded);
		m_subchunks.push_back(chunk);
	}

	return decoded;
}

Data * 
Wave::encode() const
{
	vector<Data *> encoded;
	uint32_t subchunks_length = 0;
	
	for (unsigned int i = 0; i < m_subchunks.size(); i++)
	{
		encoded.push_back(m_subchunks[i]->encode());
		subchunks_length += encoded[i]->size();
	}

	const uint32_t ID_LENGTH = 4;
	const uint32_t SIZE_LENGTH = 4;
	const uint32_t FORMAT_LENGTH = 4;
	const uint32_t HEADER_LENGTH = ID_LENGTH + SIZE_LENGTH + FORMAT_LENGTH;
	
	uint32_t data_size = FORMAT_LENGTH + subchunks_length;
	uint32_t encoded_size = HEADER_LENGTH + subchunks_length;
	// 1046524
	
	uint8_t *bytes = new uint8_t[encoded_size];
	
	if (bytes == 0)
		return 0;
		
	uint32_t offset = 0;
	
	memcpy(bytes + offset, Wave::id.c_str(), ID_LENGTH);
	offset += ID_LENGTH;
	
	memcpy(bytes + offset, &data_size, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	memcpy(bytes + offset, "WAVE", FORMAT_LENGTH);
	offset += FORMAT_LENGTH;
	 
	for (unsigned int i = 0; i < encoded.size(); i++)
	{
		memcpy(bytes + offset, encoded[i]->bytes(), encoded[i]->size());
		offset += encoded[i]->size();
	}
	
	for (unsigned int i = 0; i < encoded.size(); i++)
	{
		delete encoded[i];
	}

	Data *data = new Data();
	
	if (data == 0)
	{
		delete [] bytes;
		return 0;
	}
	
	data->set(bytes, encoded_size);
	
	return data;
}

void 
Wave::save(const string& path) const
{
	ofstream file(path.c_str());

	Data *encoded = encode();

	file.write((char *)encoded->bytes(), encoded->size());
	
	file.close();
}

void 
Wave::add_chunk(Chunk *chunk)
{
	m_subchunks.push_back(chunk);
}

Format *
Wave::format() const
{
	int chunks = m_subchunks.size();
	Format *format = 0;

	for (int i = 0; i < chunks; i++)
	{
		format = dynamic_cast<Format *>(m_subchunks[i]);

		if (format != 0)
			return format;
	}

	return format;
}

Generic *
Wave::data() const
{
	int chunks = m_subchunks.size();
	Generic *data = 0;

	for (int i = 0; i < chunks; i++)
	{
		data = dynamic_cast<Generic *>(m_subchunks[i]);

		if (data && data->id() == "data")
			break;
	}

	return data;
}

SDL_AudioSpec 
Wave::spec() const
{  
	Format *fmt = format();
	SDL_AudioSpec spec;

	spec.freq = fmt->sampleRate();
	spec.channels = fmt->numChannels(); 
	spec.samples = 4096;
	spec.size = 0;

	if (fmt->bitsPerSample() == 8)
		spec.format = AUDIO_U8;
	else
		spec.format = AUDIO_S16;

	return spec; 
}

SDL_AudioSpec* Load_Wave(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len)
{	
	Wave *wave = Wave::load(file);
	SDL_AudioSpec s = wave->spec();

	spec->format = s.format;
	spec->freq = s.freq;
	spec->channels = s.channels;
	spec->samples = 4096;
	spec->size = 0;
 
	Generic *data = wave->data();

	*audio_len = data->size();
	*audio_buf = (uint8_t *) malloc(*audio_len);
  
	memcpy(*audio_buf, data->data(), *audio_len);

	return spec;
}

