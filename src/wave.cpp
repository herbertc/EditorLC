#include <fstream>
#include <string.h>
#include "wave.h" 
#include "chunkfactory.h"

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
	
	uint32_t data_size = ID_LENGTH + subchunks_length;
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

Uint16 readShort(unsigned char **data)
{
	Uint16 value = 0;

	for (int i = 1; i >= 0; i--)
	{
		value <<= 8;
		value += (*data)[i];

	}

	*data += 2;

	return value;
}

Uint32 readInt(unsigned char **data)
{
	Uint32 value = 0;

	for (int i = 3; i >= 0; i--)
	{
		value <<= 8;
		value += (*data)[i];
	}

	*data += 4;

	return value;
}

Wave_Header* create_wave_header()
{
  return (Wave_Header *) calloc(1, sizeof(Wave_Header));
}

Subchunk1* create_subchunk1()
{
  return (Subchunk1*) calloc(1, sizeof(Subchunk1));
}

Subchunk2* create_subchunk2()
{
  return (Subchunk2*) calloc(1, sizeof(Subchunk2));
}

wave_fmt* create_wave_fmt()
{
  return (wave_fmt*) calloc(1, sizeof(wave_fmt));
}

subchunk* create_subchunk()
{
  return (subchunk*) calloc(1, sizeof(subchunk));
}

void load_wav_header(Wave_Header* wh, FILE *wave_file)
{
  if(!fread(wh->chunkID, 1, 4, wave_file))
  {
    cout << "Erro: " << stderr << endl;
    return;
  }
  
  if(!fread(&wh->chunkSize, 4, 1, wave_file))
  {
    cout << "Erro: " << stderr << endl;
    return;
  }
  
  if(!fread(wh->format, 1, 4, wave_file))
  {
    cout << "Erro: " << stderr << endl;
    return;
  }
  
}

void load_subchunk1(Subchunk1* sc1, FILE *F)
{
  fread(sc1->subchunk1ID, 1, 4, F);
  fread(&sc1->subchunk1Size, 4, 1, F);
  fread(&sc1->audioFormat, 2, 1, F);
  fread(&sc1->numChannels, 2, 1, F);
  fread(&sc1->sampleRate, 4, 1, F);
  fread(&sc1->byteRate, 4, 1, F);
  fread(&sc1->blockAlign, 2, 1, F);
  fread(&sc1->bitsperSample, 2, 1, F);
}

void load_subchunk2(Subchunk2* sc2, FILE *F)
{
  fread(sc2->subchunk2ID, 1, 4, F);
  fread(&sc2->subchunk2Size, 4, 1, F);
  
  sc2->data = (char *) calloc(1, sc2->subchunk2Size*sizeof(char));
  
  if(sc2->data == NULL)
    return;
  
  fread(sc2->data, sc2->subchunk2Size, 1, F);
  
}

int load_subchunk(subchunk *sc, const char * id, FILE *F)
{
  rewind(F);
  sc->size = 12;
  
  do
  {
    fseek(F, sc->size, SEEK_CUR);
    fread(sc->id, 1, 4, F);
    fread(&sc->size, 4, 1, F);
    
  }while(memcmp(sc->id, id, 4));
  
  sc->data = (unsigned char *) calloc(1, sc->size*sizeof(unsigned char));
  
  if(sc->data == NULL)
    return -1;
  
  fread(sc->data, sc->size, 1, F);
  
  return 0;
}
  
long wave_size(const char* path)
{
  long size;
  
  FILE* wave_file = fopen(path, "r");
  
  if(wave_file == NULL)
    return -1;
  
  fseek(wave_file, 0, SEEK_END);
  size = ftell(wave_file);
  
  fclose(wave_file);
  
  return size;
}

int parse_fmt_data(wave_fmt* fmt, const subchunk* sc)
{
	if (memcmp(sc->id, "fmt ", 4))
		return -1;

	unsigned char *data = sc->data;
	unsigned char *end = data + sc->size;

	for (; data != end; data++)
	{
		printf("%02x ", *data);
	}
	printf("\n");

	data = sc->data;

	fmt->audioFormat = readShort(&data);
	fmt->numChannels = readShort(&data);
	fmt->sampleRate = readInt(&data);
	fmt->byteRate = readInt(&data);
	fmt->blockAlign = readShort(&data);
	fmt->bitsperSample = readShort(&data);

	if (fmt->bitsperSample == 8)
		fmt->audioFormat = AUDIO_U8;
	else
		fmt->audioFormat = AUDIO_S16;

	return 0;
   
}

void* parse_wave_data(subchunk *sc)
{
  void* data = malloc(sc->size);
  memcpy(data, sc->data, sc->size);
 
  return data;
}

void print_header_file(Wave_Header *wh)
{
  
  char buffer[5];
  buffer[4] = '\0';
  
  cout << "\n[WAVE HEADER]" << endl;
  
  memcpy(buffer, wh->chunkID, 4); 
  cout << "\tchunkID: " << "[" << buffer << "]" << endl;
  
  cout << "\tchunkSize: " << wh->chunkSize << endl;
  
  memcpy(buffer, wh->format, 4);
  cout << "\tformat: " << "["  << wh->format << "]" << endl;
  
}

void print_subchunk1(Subchunk1 *sc1)
{
  char buffer[5];
  buffer[4] = '\0';
   
  cout << "\nBLOCO: [fmt ]" << endl;
  
  memcpy(buffer, sc1->subchunk1ID, 4);
  cout << "\tsubchunk1ID: " << "[" << buffer << "]" << endl;
  
  cout <<"\tsubchunk1Size: " << sc1->subchunk1Size << endl;
  cout << "\taudioFormat: " << sc1->audioFormat << endl;
  cout << "\tnumChannels: " << sc1->numChannels << endl;
  cout << "\tsampleRate: " << sc1->sampleRate << endl;
  cout << "\tbyteRate: " << sc1->byteRate << endl;
  cout << "\tblockAlign: " << sc1->blockAlign << endl;
  cout << "\tbitsperSample: " << sc1->bitsperSample << endl;
  
}

void print_subchunk2(Subchunk2 *sc2)
{
  char buffer[5];
  buffer[4] = '\0';
  
  cout << "\nBLOCO: [data]" << endl;
  
  memcpy(buffer, sc2->subchunk2ID, 4);
  cout << "\tsubchunk2ID: " << "[" << buffer << "]" << endl;
  
  cout << "\tsubchunk2Size: " << sc2->subchunk2Size << endl;
  
  cout << "\tdata: " << sc2->data << endl;
  
}


SDL_AudioSpec* Load_Wave(const char *file, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len)
{
  FILE *wave_file = fopen(file, "rb");
  
  if(wave_file == NULL)
    return NULL;
  
 //Wave_Header* wave_header = create_wave_header();
  wave_fmt* fmt = create_wave_fmt();
  subchunk* sc = create_subchunk();
  Uint8* wave_data;
	
  //load_wav_header(wave_header, wave_file);
  load_subchunk(sc, "fmt " , wave_file);

	int rc = parse_fmt_data(fmt, sc);

	if (rc != 0)
	{
		cout << "Erro no parse do fmt" << endl;
		return NULL;
	}		
  
  load_subchunk(sc, "data" , wave_file);
  wave_data = (Uint8*) parse_wave_data(sc);
  
  spec->format = fmt->audioFormat;
  spec->freq = fmt->sampleRate;
  spec->channels = fmt->numChannels; // ERRO CABULOSO!!!
  //spec->silence = não sei...;
  //spec->samples = (8*sc->size)/(fmt->numChannels*fmt->bitsperSample);
  spec->samples = 4096;
//  spec->size = sc->size;
  spec->size = 0;
  
  *audio_buf = wave_data;
  
  *audio_len = sc->size;
  
  return spec;
}


