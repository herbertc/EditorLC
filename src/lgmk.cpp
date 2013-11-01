#include "lgmk.h"

const string Lgmk::id = "LGMK";

Lgmk::Lgmk()
{}

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
	
	for (vector<uint32_t>::const_iterator it = m_marks.begin(); it != m_marks.end(); it++)
	{
		uint32_t mark = *it;
		memcpy(bytes + offset, &mark, 4);
		offset += 4;
	}

	//m_subMarks
	uint32_t subMarksCount = m_subMarks.size();
	memcpy(bytes + offset, &subMarksCount, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	for (vector<uint32_t>::const_iterator it = m_subMarks.begin(); it != m_subMarks.end(); it++)
	{
		uint32_t mark = *it;
		memcpy(bytes + offset, &mark, 4);
		offset += 4;
	}

	//m_marksNames
	uint32_t marksNamesCount = m_marksNames.size();
	memcpy(bytes + offset, &marksNamesCount, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	for(vector<string>::const_iterator it = m_marksNames.begin(); it != m_marksNames.end(); it++)
	{
		uint32_t marksSize = (*it).size();
		memcpy(bytes + offset, &marksSize, SIZE_LENGTH);
		offset += SIZE_LENGTH;

		string markName = *it;
		memcpy(bytes + offset, markName.c_str(), markName.size());
		offset += markName.size();
	}


	//m_marksNames
	uint32_t subMarksNamesCount = m_subMarksNames.size();
	memcpy(bytes + offset, &subMarksNamesCount, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	for(vector<string>::const_iterator it = m_subMarksNames.begin(); it != m_subMarksNames.end(); it++)
	{
		uint32_t marksSize = (*it).size();
		memcpy(bytes + offset, &marksSize, SIZE_LENGTH);
		offset += SIZE_LENGTH;

		string markName = *it;
		memcpy(bytes + offset, markName.c_str(), markName.size());
		offset += markName.size();
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

	uint32_t subMarks;
	memcpy(&subMarks, iterator + decoded, 4);
	decoded += 4;

	for (uint32_t i = 0; i < subMarks; i++)
	{
		uint32_t mark;
		memcpy(&mark, iterator + decoded, 4);
		decoded += 4;
		m_subMarks.push_back(mark);
	}

	uint32_t marksNames;
	memcpy(&marksNames, iterator + decoded, 4);
	decoded += 4;
	
	uint32_t temp_size;
	
	cout << "Marks Names: " <<marksNames <<endl;
	for(uint32_t i = 0; i < marksNames; i++)
	{
		memcpy(&temp_size, iterator + decoded, 4);
		decoded += 4;

		cout << "temp_size: "<< temp_size <<endl;
		
		char *temp_char = new char[temp_size + 1];
		
		if(temp_char != 0)
		{

			temp_char[temp_size] = 0;
			memcpy(temp_char, iterator + decoded, temp_size);
			
			string mark(temp_char);
	
			cout <<"temp_char: {" << mark << "}"<<endl;
			
			m_marksNames.push_back(mark);
			
			delete [] temp_char;
		}
		
		decoded += temp_size;
	}

	
	uint32_t subMarksNames;
	memcpy(&subMarksNames, iterator + decoded, 4);
	decoded += 4;
	
	uint32_t temp_size2;
	
	cout << "SubMarks Names: " << subMarksNames <<endl;
	for(uint32_t i = 0; i < subMarksNames; i++)
	{
		memcpy(&temp_size2, iterator + decoded, 4);
		decoded += 4;

		cout << "temp_size2: "<< temp_size2 <<endl;
		
		char *temp_char = new char[temp_size2 + 1];
		
		if(temp_char != 0)
		{

			temp_char[temp_size2] = 0;
			memcpy(temp_char, iterator + decoded, temp_size2);
			
			string mark(temp_char);
	
			cout <<"temp_char: {" << mark << "}"<<endl;
			
			m_subMarksNames.push_back(mark);
			
			delete [] temp_char;
		}
		
		decoded += temp_size2;
	}

	return decoded;
}

vector<uint32_t>
Lgmk::marks() const
{
	return m_marks;
}

vector<string> 
Lgmk::marksNames() const
{
	return m_marksNames;
}

vector<uint32_t> 
Lgmk::subMarks() const
{
	return m_subMarks;
}

vector<string> 
Lgmk::subMarksNames() const
{
	return m_subMarksNames;
}


void 
Lgmk::add_mark(uint32_t position)
{
	m_marks.push_back(position);
	sort(m_marks.begin(), m_marks.begin() + m_marks.size());
}

void 
Lgmk::add_markName(string name)
{
	m_marksNames.push_back(name);
}

void 
Lgmk::add_subMark(uint32_t position)
{
	m_subMarks.push_back(position);
	sort(m_subMarks.begin(), m_subMarks.begin() + m_subMarks.size());
}

void 
Lgmk::add_subMarkName(string name)
{
	m_subMarksNames.push_back(name);	
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

	os << '\n';

	os << "\t\tsubMarks (" << m_subMarks.size() << "): ";
	for(uint32_t j = 0; j < m_subMarks.size(); j++)
	{
		if (j)
			os << ", ";
		
		os << m_subMarks[j];
		
	}
	
	os << '\n';
	
	os << "\t\tmarks names (" << m_marksNames.size() << "): ";
	for(uint32_t i = 0; i < m_marksNames.size(); i++)
	{
		if (i)
			os << ", ";

		os << m_marksNames[i];
	}

	os << '\n';

	os << "\t\tsubMarks names (" << m_subMarksNames.size() << "): ";
	for(uint32_t j = 0; j < m_subMarksNames.size(); j++)
	{
		if (j)
			os << ", ";
		
		os << m_subMarksNames[j];
	}
}
	
uint32_t 
Lgmk::size() const
{
	uint32_t stringMarkSize = 0;
	uint32_t stringSubMarkSize = 0;

	for(uint32_t i = 0; i < m_marksNames.size(); i++)
	{
		stringMarkSize += m_marksNames[i].size();
	}

	for(uint32_t i = 0; i < m_subMarksNames.size(); i++)
	{
		stringSubMarkSize += m_subMarksNames[i].size();
	}

	return (2*sizeof(uint32_t) + 2*sizeof(uint32_t) * m_marks.size() + stringMarkSize) + (2*sizeof(uint32_t) + 2*sizeof(uint32_t) * m_subMarks.size() + stringSubMarkSize);
}


