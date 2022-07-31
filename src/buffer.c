
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "encoding.h"
#include "typedef.h"
#include "buffer.h"

Buffer makeBuffer(size_t size, size_t cap){
	Buffer buf = {
		.size = size,
		.cap = cap,
		.rd = 0,
		.p = malloc(cap),
	};
	return buf;
}

void growBuffer(Buffer *b, size_t l){
	if(b->cap < 1024){
		while(b->size + l > b->cap){
			b->cap = b->cap / 2 * 3;
		}
	}else if(b->size + l > b->cap){
		b->cap += (l / 1024 + 1) * 1024;
	}
	b->p = realloc(b->p, b->cap);
}

void writeBoolBuf(Buffer *b, bool_t v){
	growBuffer(b, 1);
	b->p[b->size++] = v ?1 :0;
}

void writeUint8Buf(Buffer *b, uint8_t v){
	growBuffer(b, 1);
	b->p[b->size++] = v;
}

void writeUint16Buf(Buffer *b, uint16_t v){
	growBuffer(b, 2);
	encodeUint16(b->p + b->size, v);
	b->size += 2;
}

void writeUint32Buf(Buffer *b, uint32_t v){
	growBuffer(b, 4);
	encodeUint32(b->p + b->size, v);
	b->size += 4;
}

void writeUint64Buf(Buffer *b, uint64_t v){
	growBuffer(b, 8);
	encodeUint64(b->p + b->size, v);
	b->size += 8;
}

void writeFloat32Buf(Buffer *b, float32_t v){
	growBuffer(b, 4);
	encodeFloat32(b->p + b->size, v);
	b->size += 4;
}

void writeFloat64Buf(Buffer *b, float64_t v){
	growBuffer(b, 8);
	encodeFloat64(b->p + b->size, v);
	b->size += 8;
}

void writeStringBuf(Buffer *b, const char *s){
	const size_t l = strlen(s);
	writeUint32Buf(b, (uint32_t)(l));
	if(l){
		growBuffer(b, l);
		memcpy(b->p + b->size, s, l);
		b->size += l;
	}
	return;
}


bool_t readBoolBuf(Buffer *b){
	return (b->p[b->rd++] != 0)?TRUE :FALSE;
}

uint8_t readUint8Buf(Buffer *b){
	return b->p[b->rd++];
}

uint16_t readUint16Buf(Buffer *b){
	return decodeUint16(b->p + (b->rd += 2) - 2);
}

uint32_t readUint32Buf(Buffer *b){
	return decodeUint32(b->p + (b->rd += 4) - 4);
}

uint64_t readUint64Buf(Buffer *b){
	return decodeUint64(b->p + (b->rd += 8) - 8);
}

float32_t readFloat32Buf(Buffer *b){
	return decodeFloat32(b->p + (b->rd += 4) - 4);
}

float64_t readFloat64Buf(Buffer *b){
	return decodeFloat64(b->p + (b->rd += 8) - 8);
}

size_t readStringBuf(Buffer *b, const char **s){
	uint32_t size = readUint32Buf(b);
	char *t = malloc(size + 1);
	t[size] = 0;
	memcpy(t, b->p + (b->rd += size) - size, size);
	*s = t;
	return size;
}


size_t putStringBuf(Buffer *b, const char *s){
	const size_t l = strlen(s);
	if(l){
		growBuffer(b, l);
		memcpy(b->p + b->size, s, l);
		b->size += l;
	}
	return l;
}

void getStringBuf(Buffer *b, char **s, size_t size){
	memcpy(*s, b->p + (b->rd += size) - size, size);
}

size_t writeBufTo(Buffer *b, FILE *fd){
	return fwrite(b->p, 1, b->size, fd);
}

size_t readBufFrom(Buffer *b, FILE *fd, size_t size){
	growBuffer(b, size);
	b->rd = 0;
	b->size = fread(b->p, 1, size, fd);
	return b->size;
}
