
#include <stdio.h>
#include <strings.h>

#include <typedef.h>
#include "encoding.h"

void encodeUint16(uint8_t *buf, uint16_t v){
	buf[0] = v & 0xff;
	buf[1] = (v >> 8) & 0xff;
}

void encodeUint32(uint8_t *buf, uint32_t v){
	buf[0] = v & 0xff;
	buf[1] = (v >> 8) & 0xff;
	buf[2] = (v >> 16) & 0xff;
	buf[3] = (v >> 24) & 0xff;
}

void encodeUint64(uint8_t *buf, uint64_t v){
	buf[0] = v & 0xff;
	buf[1] = (v >> 8) & 0xff;
	buf[2] = (v >> 16) & 0xff;
	buf[3] = (v >> 24) & 0xff;
	buf[4] = (v >> 32) & 0xff;
	buf[5] = (v >> 40) & 0xff;
	buf[6] = (v >> 48) & 0xff;
	buf[7] = (v >> 56) & 0xff;
}

void encodeFloat32(uint8_t *buf, float32_t v){
	encodeUint32(buf, *(uint32_t*)(&v));
}

void encodeFloat64(uint8_t *buf, float64_t v){
	encodeUint64(buf, *(uint64_t*)(&v));
}

uint16_t decodeUint16(const uint8_t *buf){
	uint16_t v;
	v = (uint16_t)(buf[0]) | ((uint16_t)(buf[1]) << 8);
	return v;
}

uint32_t decodeUint32(const uint8_t *buf){
	uint32_t v;
	v = (uint32_t)(buf[0]) | ((uint32_t)(buf[1]) << 8) | ((uint32_t)(buf[2]) << 16) | ((uint32_t)(buf[3]) << 24);
	return v;
}

uint64_t decodeUint64(const uint8_t *buf){
	uint64_t v;
	v = (uint64_t)(buf[0]) | ((uint64_t)(buf[1]) << 8) | ((uint64_t)(buf[2]) << 16) | ((uint64_t)(buf[3]) << 24)
		 | ((uint64_t)(buf[4]) << 32) | ((uint64_t)(buf[5]) << 40) | ((uint64_t)(buf[6]) << 48) | ((uint64_t)(buf[7]) << 56);
	return v;
}

float32_t decodeFloat32(const uint8_t *buf){
	uint32_t v = decodeUint32(buf);
	return *((float32_t*)(&v));
}

float64_t decodeFloat64(const uint8_t *buf){
	uint64_t v = decodeUint64(buf);
	return *((float64_t*)(&v));
}

size_t writeBool(FILE *fd, bool_t v){
	return fputc(v ?1 :0, fd);
}

size_t writeUint8(FILE *fd, uint8_t v){
	return fputc(v, fd);
}

size_t writeUint16(FILE *fd, uint16_t v){
	uint8_t buf[2];
	encodeUint16(buf, v);
	return fwrite(buf, 1, 2, fd);
}

size_t writeUint32(FILE *fd, uint32_t v){
	uint8_t buf[4];
	encodeUint32(buf, v);
	return fwrite(buf, 1, 4, fd);
}

size_t writeUint64(FILE *fd, uint64_t v){
	uint8_t buf[8];
	encodeUint64(buf, v);
	return fwrite(buf, 1, 8, fd);
}

size_t writeFloat32(FILE *fd, float32_t v){
	return writeUint32(fd, *(uint32_t*)(&v));
}

size_t writeFloat64(FILE *fd, float64_t v){
	return writeUint64(fd, *(uint64_t*)(&v));
}

size_t readBool(FILE *fd, bool_t *v){
	int c = fgetc(fd);
	if(c == EOF){
		return 0;
	}
	*v = c ?TRUE :FALSE;
	return 1;
}

size_t readUint8(FILE *fd, uint8_t *v){
	int c = fgetc(fd);
	if(c == EOF){
		return 0;
	}
	*v = c;
	return 1;
}

size_t readUint16(FILE *fd, uint16_t *v){
	uint8_t buf[2];
	size_t n = fread(buf, 1, 2, fd);
	if(n != 2){
		return n;
	}
	*v = decodeUint16(buf);
	return n;
}

size_t readUint32(FILE *fd, uint32_t *v){
	uint8_t buf[4];
	size_t n = fread(buf, 1, 4, fd);
	if(n != 4){
		return n;
	}
	*v = decodeUint32(buf);
	return n;
}

size_t readUint64(FILE *fd, uint64_t *v){
	uint8_t buf[8];
	size_t n = fread(buf, 1, 8, fd);
	if(n != 8){
		return n;
	}
	*v = decodeUint64(buf);
	return n;
}

size_t readFloat32(FILE *fd, float32_t *v){
	return readUint32(fd, (uint32_t*)(v));
}

size_t readFloat64(FILE *fd, float64_t *v){
	return readUint64(fd, (uint64_t*)(v));
}

size_t writeString(FILE *fd, const char *v){
	return writeBytes(fd, (const uint8_t*)(v), strlen(v));
}

size_t writeBytes(FILE *fd, const uint8_t *v, size_t size){
	size_t n = writeUint32(fd, (uint32_t)(size));
	n += fwrite(v, 1, size, fd);
	return n;
}
