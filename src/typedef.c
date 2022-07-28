
#include "typedef.h"

slice_t _newSlice(size_t elem, size_t size, size_t cap){
	if(size > cap){
		cap = size;
	}
	slice_t s = {
		.size = size,
		.cap = cap,
		.p = (void*)(malloc(cap * elem)),
	};
	return s;
}
