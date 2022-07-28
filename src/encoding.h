
#ifndef __ENCODING_H__
#define __ENCODING_H__

#include "typedef.h"

void encodeUint16(uint8_t *buf, uint16_t v);
void encodeUint32(uint8_t *buf, uint32_t v);
void encodeUint64(uint8_t *buf, uint64_t v);
void encodeFloat32(uint8_t *buf, float32_t v);
void encodeFloat64(uint8_t *buf, float64_t v);

uint16_t decodeUint16(const uint8_t *buf);
uint32_t decodeUint32(const uint8_t *buf);
uint64_t decodeUint64(const uint8_t *buf);
float32_t decodeFloat32(const uint8_t *buf);
float64_t decodeFloat64(const uint8_t *buf);

size_t writeBool(FILE *fd, bool_t v);
size_t writeUint8(FILE *fd, uint8_t v);
size_t writeUint16(FILE *fd, uint16_t v);
size_t writeUint32(FILE *fd, uint32_t v);
size_t writeUint64(FILE *fd, uint64_t v);
size_t writeFloat32(FILE *fd, float32_t v);
size_t writeFloat64(FILE *fd, float64_t v);

size_t readBool(FILE *fd, bool_t *v);
size_t readUint8(FILE *fd, uint8_t *v);
size_t readUint16(FILE *fd, uint16_t *v);
size_t readUint32(FILE *fd, uint32_t *v);
size_t readUint64(FILE *fd, uint64_t *v);
size_t readFloat32(FILE *fd, float32_t *v);
size_t readFloat64(FILE *fd, float64_t *v);

size_t writeString(FILE *fd, const char *v);
size_t writeBytes(FILE *fd, const uint8_t *v, size_t size);

size_t readString(FILE *fd, char **v);
size_t readBytes(FILE *fd, uint8_t **v, size_t *size);

#endif // __ENCODING_H__
