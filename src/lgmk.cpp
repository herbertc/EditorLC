#include "lgmk.h"

const string Lgmk::id = "LGMK";

Lgmk::Lgmk()
{
}

Data *
Lgmk::encode() const
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
	
	memcpy(bytes + offset, Lgmk::id.c_str(), ID_LENGTH);
	offset += ID_LENGTH;
	
	memcpy(bytes + offset, &encoded_size, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	//m_marks
	uint32_t marksCount = m_marks.size();
	memcpy(bytes + offset, &marksCount, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	for (vector<uint32_t>::const_iterator it = m_marks.begin(); it != m_marks.end();
		it++)
	{
		uint32_t mark = *it;
		memcpy(bytes + offset, &mark, 4);
		offset += 4;
	}

	Data *data = new Data();
	data->set(bytes, total_size);

	return data;
}

uint32_t 
Lgmk::decode(const Data& data, uint32_t offset)
{
	const uint8_t *iterator = data.bytes(offset);
	uint32_t decoded = 0;
	
	// Read id
	char buffer[5];
	memcpy(buffer, iterator + decoded, 4);
	buffer[4] = 0;
	
	if (memcmp(buffer, Lgmk::id.c_str(), 4))
		return 0;
	
	decoded += 4;
	
	uint32_t size;
	memcpy(&size, iterator + decoded, 4);
	decoded += 4;
	
	uint32_t marks;
	memcpy(&marks, iterator + decoded, 4);
	decoded += 4;

	for (uint32_t i = 0; i < marks; i++)
	{
		uint32_t mark;
		memcpy(&mark, iterator + decoded, 4);
		decoded += 4;
		m_marks.push_back(mark);
	}

	return decoded;
}

vector<uint32_t>
Lgmk::marks() const
{
	return m_marks;
}

void 
Lgmk::add_mark(uint32_t position)
{
	m_marks.push_back(position);
	sort(m_marks.begin(), m_marks.begin() + m_marks.size());
}

void 
Lgmk::print(ostream& os) const
{
	os << "\tID: " << Lgmk::id << endl;
	os << "\tsize: " << size() << endl;
	
	os << "\t\tmarks (" << m_marks.size() << "): ";
	for(uint32_t i = 0; i < m_marks.size(); i++)
	{
		if (i)
			os << ", ";

		os << m_marks[i];
	}

}
	
uint32_t 
Lgmk::size() const
{
	return sizeof(uint32_t) + sizeof(uint32_t) * m_marks.size();
}
	
