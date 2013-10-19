#include "meta.h"

const string Meta::id = "META";

Meta::Meta()
{
	for (int i = 0; i <= COVER; i++)
	{
		fields.push_back("Unknown");
	}
}
	
Data * 
Meta::encode() const
{
	const int ID_LENGTH = 4;
	const int SIZE_LENGTH = 4;
	const int HEADER_LENGTH = ID_LENGTH + SIZE_LENGTH;
	uint32_t encoded_size = size() + fields.size()*SIZE_LENGTH;
	
	uint32_t total_size = encoded_size + HEADER_LENGTH;
	
	uint8_t *bytes = new uint8_t[total_size];

	if (bytes == 0)
		return 0;

	uint32_t offset = 0;
	
	memcpy(bytes + offset, Meta::id.c_str(), ID_LENGTH);
	offset += ID_LENGTH;
	
	memcpy(bytes + offset, &encoded_size, SIZE_LENGTH);
	offset += SIZE_LENGTH;
	
	uint32_t fieldsCount = fields.size();
	
	for(uint32_t i = 0; i < fieldsCount; i++)
	{
		int size = fields[i].size();

		memcpy(bytes + offset, &size, SIZE_LENGTH);
		offset += SIZE_LENGTH;
		
		memcpy(bytes + offset, fields[i].c_str(), size); 
		offset += size;
	}

	Data *data = new Data();
	data->set(bytes, total_size);

	return data;
}

uint32_t 
Meta::decode(const Data& data, uint32_t offset)
{
	const uint8_t *iterator = data.bytes(offset);
	uint32_t decoded = 0;
	
	// Read id
	char buffer[5];
	memcpy(buffer, iterator + decoded, 4);
	buffer[4] = 0;

	if (memcmp(buffer, Meta::id.c_str(), 4))
		return 0;
	
	decoded += 4;	

	// Read size
	uint32_t size;
	memcpy(&size, iterator + decoded, 4);
	decoded += 4;

	uint32_t temp_size;
	uint32_t fieldsCount = fields.size();

	for(uint32_t i = 0; i < fieldsCount; i++)
	{
		memcpy(&temp_size, iterator + decoded, 4);
		decoded += 4;
		
		char *temp_char = new char[temp_size + 1];
		
		if(temp_char != 0)
		{
			temp_char[temp_size] = 0;
			memcpy(temp_char, iterator + decoded, temp_size);
			fields[i] = temp_char;
			
			delete [] temp_char;
		}
		
		decoded += temp_size;
	}

	return decoded;	
}

void 
Meta::print(ostream& os) const
{
	os << "\tID: " << Meta::id << endl;
	os << "\tsize: " << size() << endl;
	os << "\t\ttitle: [" << fields[TITLE] << "]" << endl;
	os << "\t\tauthor: [" << fields[AUTHOR] << "]" << endl;
	os << "\t\tlanguage: [" << fields[LANGUAGE] << "]" << endl;
	os << "\t\tpublisher: [" << fields[PUBLISHER] << "]"<< endl;
	os << "\t\tyear: [" << fields[YEAR] << "]"<< endl;
	os << "\t\taddress: [" << fields[ADDRESS] << "]"<< endl;
	os << "\t\tpages: [" << fields[PAGES] << "]"<< endl;
}

uint32_t 
Meta::size() const
{
	uint32_t total = 0;
	uint32_t fieldsCount = fields.size();
	
	for (uint32_t i = 0; i < fieldsCount; i++)
	{
		total += fields[i].size();
	}
		
	return total;
}

void 
Meta::setTitle(const string& title)
{
	fields[TITLE] = title;
}
	
void
Meta::setAuthor(const string& author)
{
	fields[AUTHOR] = author;
}

void 
Meta::setLanguage(const string& language)
{
	fields[LANGUAGE] = language;
}

void 
Meta::setPublisher(const string& publisher)
{
	fields[PUBLISHER] = publisher;
}

void 
Meta::setYear(const string& year)
{
	fields[YEAR] = year;
}

void 
Meta::setAddress(const string& address)
{
	fields[ADDRESS] = address;
}

void 
Meta::setpages(const string& pages)
{
	fields[PAGES] = pages;
}

string 
Meta::title() const
{
	return fields[TITLE];
}

string
Meta::author() const
{
	return fields[AUTHOR];
}

string 
Meta::language() const
{
	return fields[LANGUAGE];
}

string 
Meta::publisher() const
{
	return fields[PUBLISHER];
}

string 
Meta::year() const
{
	return fields[YEAR];
}

string 
Meta::address() const
{
	return fields[ADDRESS];
}

string 
Meta::pages () const
{
	return fields[PAGES];
}

