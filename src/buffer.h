
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdio.h>

#include "typedef.h"

typedef struct{
	size_t size;
	size_t cap;
	size_t rd;
	uint8_t *p;
} Buffer;

Buffer makeBuffer(size_t size, size_t cap);

void growBuffer(Buffer *b, size_t l);

void writeBoolBuf(Buffer *b, bool_t v);
void writeUint8Buf(Buffer *b, uint8_t v);
void writeUint16Buf(Buffer *b, uint16_t v);
void writeUint32Buf(Buffer *b, uint32_t v);
void writeUint64Buf(Buffer *b, uint64_t v);
void writeFloat32Buf(Buffer *b, float32_t v);
void writeFloat64Buf(Buffer *b, float64_t v);
size_t writeStringBuf(Buffer *b, const char *s);

size_t putStringBuf(Buffer *b, const char *s);
size_t writeBufTo(Buffer *b, FILE *fd);
size_t readBufFrom(Buffer *b, FILE *fd, size_t size);

#define freeBuffer(buf) do{ if(buf.p != NULL){ free(buf.p); buf.p = NULL; } }while(0)

#endif // __BUFFER_H__
