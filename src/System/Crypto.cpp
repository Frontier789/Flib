#include <FRONTIER/System/Crypto.hpp>

#include "Generic/picosha2.h"

namespace fm
{
	std::string sha256(const void *data,fm::Size bytes) {
		uint8_t *ptr = (uint8_t*)data;
		
		return picosha2::hash256_hex_string(ptr,ptr + bytes);
	}
}
