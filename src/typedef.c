
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

size_t *slice_to_sizes(slice_t sli, size_t *target){
	for(size_t i = 0; i < sli.size; ++i){
		target[i] = (size_t)(slice_get(sli, i, uint64_t));
	}
	return target;
}
