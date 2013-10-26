#ifndef LGMK_H
#define LGMK_H

#include <string.h>

#include <stdint.h>

#include <algorithm>
#include <vector>
#include <string>

#include "chunk.h"

class Lgmk : public Chunk
{
public:
	Lgmk();
	
	Data *encode() const;
	uint32_t decode(const Data& data, uint32_t offset = 0);
	
	void print(ostream& os) const;
	
	vector<uint32_t> marks() const;
	vector<string> marksNames() const;

	vector<uint32_t> subMarks() const;
	vector<string> subMarksNames() const;
	
	void add_mark(uint32_t position);
	void add_markName(string name);

	void add_subMark(uint32_t position);
	void add_subMarkName(string name);
	
	static const string id;
private:
	vector<uint32_t> m_marks;
	vector<uint32_t> m_subMarks;
	
	vector<string> m_marksNames;
	vector<string> m_subMarksNames;

	uint32_t size() const;
};

#endif

