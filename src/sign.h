
#ifndef __SIGN_H__
#define __SIGN_H__

#include "typedef.h"
#include "buffer.h"

enum SignCh{
	pointer = '*',
	array = '[',
	bool = 'A',
	uint8 = 'B',
	uint16 = 'I',
	uint32 = 'L',
	uint64 = 'Q',
	int8 = 'b',
	int16 = 'i',
	int32 = 'l',
	int64 = 'q',
	float32 = 'F',
	float64 = 'D',
	string = 'S',
};

typedef struct{
	const char *s;
	any_t v;
} signed_ptr;

size_t get_size(enum SignCh t);
size_t signCount(const char *sign);

size_t writeValue(FILE *fd, const char *sign, any_t val);
size_t writeValueBuf(Buffer *b, const char *sign, any_t val);

size_t readValue0(FILE *fd, const char **sign, any_t val);
size_t readValue(FILE *fd, const char *sign, any_t val);
void *readValue1(FILE *fd, const char *sign, slice_t *ptrs);

void freeArgsWithSign(void *args, const char *sign);

#endif // __SIGN_H__
