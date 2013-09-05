#ifndef META_H
#define	META_H

#include <string.h>

#include <string>
#include <vector>

#include "chunk.h"

class Meta : public Chunk {
public:
	Meta();
	
	Data * encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);

	void print(ostream& os) const;
	
	void setTitle(const string& title);
	void setAuthor(const string& author);
	void setLanguage(const string& language);
	void setPublisher(const string& publisher);
	void setYear(const string& year);
	void setAddress(const string& address);
	void setpages(const string& pages);
	
	string title() const;
	string author() const;
	string language() const;
	string publisher() const;
	string year() const;
	string address() const;
	string pages () const;
	
	static const string id;
	
private:
	enum {TITLE = 0, AUTHOR, LANGUAGE, PUBLISHER, YEAR, ADDRESS, PAGES, COVER};

	vector<string> fields;

	uint32_t size() const;
};

#endif	/* META_H */

