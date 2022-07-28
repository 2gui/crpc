
#include <stdio.h>

#include <typedef.h>
#include <encoding.h>
#include <buffer.h>
#include "sign.h"

size_t get_size(enum SignCh t){
	switch(t){
	case pointer:
	case array:
		return sizeof(void *);
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

size_t writeValue(FILE *fd, const char *sign, const any_t val){
	switch((enum SignCh)(*sign)){
	case pointer:
		switch((enum SignCh)(sign[1])){
		case pointer:
		case array:
			return writeValue(fd, sign + 2, cast_any(val, void *));
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
		return -1;
	case array:{
		slice_t *sli = cast_any(val, slice_t *);
		size_t n = 0;
		switch((enum SignCh)(sign[1])){
		case pointer:
		case array:
			for(size_t i = 0; i < sli->size; ++i){
				n += writeValue(fd, sign + 2, slice_get(sli, i, void *));
			}
			return n;
		case bool:
			for(size_t i = 0; i < sli->size; ++i){
				n += writeBool(fd, slice_get(sli, i, bool_t));
			}
			return n;
		case uint8:
		case int8:
			for(size_t i = 0; i < sli->size; ++i){
				n += writeUint8(fd, slice_get(sli, i, uint8_t));
			}
			return n;
		case uint16:
		case int16:
			for(size_t i = 0; i < sli->size; ++i){
				n += writeUint16(fd, slice_get(sli, i, uint16_t));
			}
			return n;
		case uint32:
		case int32:
		case float32:
			for(size_t i = 0; i < sli->size; ++i){
				n += writeUint32(fd, slice_get(sli, i, uint32_t));
			}
			return n;
		case uint64:
		case int64:
		case float64:
			for(size_t i = 0; i < sli->size; ++i){
				n += writeUint64(fd, slice_get(sli, i, uint64_t));
			}
			return n;
		}
		return -1;
	}
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
	return -1;
}

size_t writeValueBuf(Buffer *b, const char *sign, const any_t val){
	switch((enum SignCh)(*sign)){
	case pointer:
		switch((enum SignCh)(sign[1])){
		case pointer:
		case array:
			return writeValueBuf(b, sign + 2, cast_any(val, void *));
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
		return -1;
	case array:{
		slice_t *sli = cast_any(val, slice_t *);
		size_t n = 0;
		switch((enum SignCh)(sign[1])){
		case pointer:
		case array:
			for(size_t i = 0; i < sli->size; ++i){
				n += writeValueBuf(b, sign + 2, slice_get(sli, i, void *));
			}
			return n;
		case bool:
			for(size_t i = 0; i < sli->size; ++i){
				writeBoolBuf(b, slice_get(sli, i, bool_t));
				n += 1;
			}
			return n;
		case uint8:
		case int8:
			for(size_t i = 0; i < sli->size; ++i){
				writeUint8Buf(b, slice_get(sli, i, uint8_t));
				n += 1;
			}
			return n;
		case uint16:
		case int16:
			for(size_t i = 0; i < sli->size; ++i){
				writeUint16Buf(b, slice_get(sli, i, uint16_t));
				n += 2;
			}
			return n;
		case uint32:
		case int32:
		case float32:
			for(size_t i = 0; i < sli->size; ++i){
				writeUint32Buf(b, slice_get(sli, i, uint32_t));
				n += 4;
			}
			return n;
		case uint64:
		case int64:
		case float64:
			for(size_t i = 0; i < sli->size; ++i){
				writeUint64Buf(b, slice_get(sli, i, uint64_t));
				n += 8;
			}
			return n;
		}
		return 0;
	}
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
	switch((enum SignCh)(*(*sign++))){
	case pointer:{
		return readValue0(fd, sign, *(any_t*)(val) = malloc(get_size((enum SignCh)(*(*sign)))));
	}
	case array:{
		size_t n = 0;
		uint32_t size;
		readUint32(fd, &size);
		slice_t *sli = (*(any_t*)(val) = malloc(sizeof(slice_t)));
		size_t elem = get_size((enum SignCh)(*(*sign++)));
		*sli = _newSlice(elem, size, size);
		for(size_t i = 0; i < size; ++i){
			n += readValue0(fd, sign, sli->p + i * elem);
		}
		return n;
	}
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
		readValue0(fd, &sign, args - size);
	}
	return args;
}
