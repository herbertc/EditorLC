#ifndef DATA_H
#define DATA_H

#include <stdint.h>

class Data {
public:
	Data();
	~Data();

	void set(uint8_t *bytes, int amount);

	uint32_t size() const;
	const uint8_t * bytes(uint32_t offset = 0) const;
	

private:
	uint32_t m_size;
	uint8_t *m_bytes;
};

#endif
