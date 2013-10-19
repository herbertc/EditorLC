#include "format.h"

const string Format::id = "fmt ";

Format::Format()
{
	m_audioFormat = 0;
	m_numChannels = 0;
	m_sampleRate = 0;
	m_byteRate = 0;
	m_blockAlign = 0;
	m_bitsperSample = 0;
}

Data *
Format::encode() const
{
	const int ID_LENGTH = 4;
	const int SIZE_LENGTH = 4;
	const int HEADER_LENGTH = ID_LENGTH + SIZE_LENGTH;
	
	uint32_t encoded_size = size(); 
	
	uint32_t total_size = encoded_size + HEADER_LENGTH;
	
	uint8_t *bytes = new uint8_t[total_size];
	
	
	if (bytes == 0)
		return 0;
	
	uint32_t offset = 0;
	
	memcpy(bytes + offset, Format::id.c_str(), ID_LENGTH);
	offset += ID_LENGTH;
	
	memcpy(bytes + offset, &encoded_size, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	//m_audioFormat
	uint16_t audioFormatSize = sizeof(m_audioFormat);
	memcpy(bytes + offset, &m_audioFormat, audioFormatSize); 
	offset += audioFormatSize;
	
	//m_numChannels
	uint16_t numChannelsSize = sizeof(m_numChannels);
	memcpy(bytes + offset, &m_numChannels, numChannelsSize); 
	offset += numChannelsSize;
	
	//m_sampleRate
	uint32_t sampleRateSize = sizeof(m_sampleRate);
	memcpy(bytes + offset, &m_sampleRate, sampleRateSize); 
	offset += sampleRateSize;
	
	//m_byteRate
	uint32_t byteRateSize = sizeof(m_byteRate);
	memcpy(bytes + offset, &m_byteRate, byteRateSize); 
	offset += byteRateSize;
	
	//m_blockAlign;
	uint16_t blockAlignSize = sizeof(m_blockAlign);
	memcpy(bytes + offset, &m_blockAlign, blockAlignSize); 
	offset += blockAlignSize;
	
	//m_bitsperSample
	uint16_t bitsperSampleSize = sizeof(m_bitsperSample);
	memcpy(bytes + offset, &m_bitsperSample, bitsperSampleSize); 
	offset += bitsperSampleSize;

	Data *data = new Data();
	data->set(bytes, total_size);

	return data;
}

uint32_t
Format::decode(const Data& data, uint32_t offset)
{
	const uint8_t *iterator = data.bytes(offset);
	uint32_t decoded = 0;
	
	// Read id
	char buffer[5];
	memcpy(buffer, iterator + decoded, 4);
	buffer[4] = 0;
	
	if (memcmp(buffer, Format::id.c_str(), 4))
		return 0;
	
	decoded += 4;
	
	uint32_t size;
	memcpy(&size, iterator + decoded, 4);
	decoded += 4;
	
	memcpy(&m_audioFormat, iterator + decoded, 2);
	decoded += 2;
	
	memcpy(&m_numChannels, iterator + decoded, 2);
	decoded += 2;
	
	memcpy(&m_sampleRate, iterator + decoded, 4);
	decoded += 4;
	
	memcpy(&m_byteRate, iterator + decoded, 4);
	decoded += 4;
	
	memcpy(&m_blockAlign, iterator + decoded, 2);
	decoded += 2;
	 
	memcpy(&m_bitsperSample, iterator + decoded, 2);
	decoded += 2;

	//if(data.size()-16 > 0)
		//decoded += (data.size()-16);

	return decoded;	
}

uint16_t 
Format::audioFormat() const
{
	return m_audioFormat;
}

uint16_t 
Format::numChannels() const
{
	return m_numChannels;
}

uint32_t 
Format::sampleRate() const
{
	return m_sampleRate;
}

void
Format::print(ostream& os) const
{
	os << "\tID: " << Format::id << endl;
	os << "\tsize: " << size() << endl;
	os << "\t\taudioFormat: [" << m_audioFormat << "]" << endl;
	os << "\t\tnumChannels: ["  << m_numChannels << "]" << endl;
	os << "\t\tsampleRate: ["  << m_sampleRate << "]" << endl;
	os << "\t\tbyteRate: ["  << m_byteRate << "]" << endl;
	os << "\t\tblockAlign: ["  << m_blockAlign << "]" << endl;
	os << "\t\tbitsperSample: ["  << m_bitsperSample << "]" << endl;
}


uint32_t
Format::size() const
{
	uint32_t s = 0;
	
	s += sizeof(m_audioFormat);
	s += sizeof(m_numChannels);
	s += sizeof(m_sampleRate);
	s += sizeof(m_byteRate); 
	s += sizeof(m_blockAlign);
	s += sizeof(m_bitsperSample);
	
	return s;
}

uint16_t 
Format::bitsPerSample() const
{
	return m_bitsperSample;
}

