#include "data.h"

Data::Data()
{
	m_bytes = 0;
	m_size = 0;
}

Data::~Data()
{
	delete [] m_bytes;
}

void 
Data::set(uint8_t *bytes, int amount)
{
	if (m_bytes)
	{
		delete [] m_bytes;
	}

	m_bytes = bytes;
	m_size = amount;
}

uint32_t 
Data::size() const
{
	return m_size;
}

const uint8_t * 
Data::bytes(uint32_t offset) const
{
	return m_bytes + offset;
}


