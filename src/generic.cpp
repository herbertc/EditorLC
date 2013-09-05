#include "generic.h"
#include "string.h"

void 
Generic::print(ostream& os) const
{
	os << "\tID: " << m_id << endl;
	os << "\tsize: " << m_size << endl;
}

Generic::Generic()
{
	m_size = 0;
	m_data = 0;
	m_id = "UNKW";
}

Generic::~Generic()
{
	delete [] m_data;
}

Data * 
Generic::encode() const
{
	const int ID_LENGTH = 4;
	const int SIZE_LENGTH = 4;
	const int HEADER_LENGTH = ID_LENGTH + SIZE_LENGTH;

	uint8_t *bytes = new uint8_t[HEADER_LENGTH + m_size];

	if (bytes == 0)
		return 0;

	memcpy(bytes, m_id.c_str(), ID_LENGTH);
	memcpy(bytes + ID_LENGTH, &m_size, SIZE_LENGTH);
	memcpy(bytes + HEADER_LENGTH, m_data, m_size);

	Data *data = new Data();
	data->set(bytes, HEADER_LENGTH + m_size);

	return data;
}

uint32_t 
Generic::decode(const Data& data, uint32_t offset)
{
	const uint8_t *iterator = data.bytes(offset);
	uint32_t decoded = 0;
	
	// Read id
	char buffer[5];
	memcpy(buffer, iterator + decoded, 4);
	buffer[4] = 0;

	m_id = string(buffer);
	decoded += 4;	

	// Read size
	memcpy(&m_size, iterator + decoded, 4);
	decoded += 4;

	// Read data bytes
	m_data = new uint8_t[m_size];

	if (m_data != 0)
	{
		memcpy(m_data, iterator + decoded, m_size);
	} 

	decoded += m_size;

	return decoded;	
}
	
string 
Generic::id() const
{
	return m_id;
}

uint32_t 
Generic::size() const
{
	return m_size;
}

const uint8_t * 
Generic::data() const
{
	return m_data;
}

