
#include "typedef.h"

slice_t _makeSlice(size_t elem, size_t size, size_t cap){
	if(size > cap){
		cap = size;
	}
	slice_t s = {
		.size = size,
		.cap = cap,
		.p = cap ?(void*)(malloc(cap * elem)) :NULL,
	};
	return s;
}
