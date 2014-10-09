
#include "types.h"

//sanity checks
char __type_error__[(

	sizeof(uint8_t)  == 1 &&
	sizeof(uint16_t) == 2 &&
	sizeof(uint32_t) == 4 &&
	sizeof(uint64_t) == 8 &&
	sizeof(uintptr_t) == 4 &&

	sizeof(size_t)   == (SYSTEM_BITS/8) &&

	sizeof(int8_t)   == 1 &&
	sizeof(int16_t)  == 2 &&
	sizeof(int32_t)  == 4 &&
	sizeof(int64_t)  == 8 &&
	sizeof(intptr_t) == 4 &&

	1) ? 0 : -1
];
