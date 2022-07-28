
#include <stdio.h>

#include "typedef.h"
#include "encoding.h"
#include "buffer.h"
#include "sign.h"

size_t get_size(enum SignCh t){
	switch(t){
	case pointer:
	case string:
		return sizeof(void *);
	case array:
		return sizeof(slice_t);
	case bool:
	case uint8:
	case int8:
		return 1;
	case uint16:
	case int16:
		return 2;
	case uint32:
	case int32:
	case float32:
		return 4;
	case uint64:
	case int64:
	case float64:
		return 8;
	}
	return 0;
}

size_t signCount(const char *sign){
	if(sign == NULL){
		return 0;
	}
	size_t c = 0;
	while(*sign){
		switch((enum SignCh)(*sign++)){
		case pointer:
		case array:
			break;
		case string:
		case bool:
		case uint8:
		case int8:
		case uint16:
		case int16:
		case uint32:
		case int32:
		case float32:
		case uint64:
		case int64:
		case float64:
			++c;
		}
	}
	return c;
}

size_t writeValue(FILE *fd, const char *sign, any_t val){
	switch((enum SignCh)(*sign++)){
	case pointer:
		switch((enum SignCh)(*sign)){
		case pointer:
		case string:
			val = cast_any(val, any_t);
			break;
		default:;
		}
		return writeValue(fd, sign, val);
	case array:{
		slice_t sli = cast_any(val, slice_t);
		size_t n = 0;
		size_t size = get_size((enum SignCh)(*sign));
		for(size_t i = 0; i < sli.size; ++i){
			n += writeValue(fd, sign, sli.p + i * size);
		}
		return n;
	}
	case string:
		return writeString(fd, (char*)(val));
	case bool:
		return writeBool(fd, cast_any(val, bool_t));
	case uint8:
	case int8:
		return writeUint8(fd, cast_any(val, uint8_t));
	case uint16:
	case int16:
		return writeUint16(fd, cast_any(val, uint16_t));
	case uint32:
	case int32:
	case float32:
		return writeUint32(fd, cast_any(val, uint32_t));
	case uint64:
	case int64:
	case float64:
		return writeUint64(fd, cast_any(val, uint64_t));
	}
	return 0;
}

size_t writeValueBuf(Buffer *b, const char *sign, any_t val){
	switch((enum SignCh)(*sign++)){
	case pointer:
		switch((enum SignCh)(*sign)){
		case pointer:
		case string:
			val = cast_any(val, any_t);
			break;
		default:;
		}
		return writeValueBuf(b, sign, val);
	case array:{
		slice_t sli = cast_any(val, slice_t);
		size_t n = 0;
		size_t size = get_size((enum SignCh)(*sign));
		for(size_t i = 0; i < sli.size; ++i){
			n += writeValueBuf(b, sign, sli.p + i * size);
		}
		return n;
	}
	case string:
		return writeStringBuf(b, (char*)(val));
	case bool:
		writeBoolBuf(b, cast_any(val, bool_t));
		return 1;
	case uint8:
	case int8:
		writeUint8Buf(b, cast_any(val, uint8_t));
		return 1;
	case uint16:
	case int16:
		writeUint16Buf(b, cast_any(val, uint16_t));
		return 2;
	case uint32:
	case int32:
	case float32:
		writeUint32Buf(b, cast_any(val, uint32_t));
		return 4;
	case uint64:
	case int64:
	case float64:
		writeUint64Buf(b, cast_any(val, uint64_t));
		return 8;
	}
	return 0;
}

size_t readValue0(FILE *fd, const char **sign, any_t val){
	switch((enum SignCh)(*((*sign)++))){
	case pointer:{
		return readValue0(fd, sign, *(any_t*)(val) = malloc(get_size((enum SignCh)(**sign))));
	}
	case array:{
		size_t n = 0;
		uint32_t size;
		readUint32(fd, &size);
		size_t elem = get_size((enum SignCh)(*(*sign)++));
		slice_t sli = _newSlice(elem, size, size);
		*(slice_t *)(val) = sli;
		for(size_t i = 0; i < size; ++i){
			n += readValue0(fd, sign, sli.p + i * elem);
		}
		return n;
	}
	case string:
		return readString(fd, (char**)(val));
	case bool:
		return readBool(fd, (bool_t*)(val));
	case uint8:
	case int8:
		return readUint8(fd, (uint8_t*)(val));
	case uint16:
	case int16:
		return readUint16(fd, (uint16_t*)(val));
	case uint32:
	case int32:
	case float32:
		return readUint32(fd, (uint32_t*)(val));
	case uint64:
	case int64:
	case float64:
		return readUint64(fd, (uint64_t*)(val));
	}
	return 0;
}

size_t readValue(FILE *fd, const char *sign, any_t val){
	return readValue0(fd, &sign, val);
}

void *readValue1(FILE *fd, const char *sign){
	void *args = malloc(0);
	size_t size;
	size_t off = 0;
	while(*sign && (size = get_size((enum SignCh)(*sign)))){
		args = realloc(args, off += size);
		readValue0(fd, &sign, args + off - size);
	}
	return args;
}
