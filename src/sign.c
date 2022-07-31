
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
			break;
		default:
			return 0;
		}
	}
	return c;
}

void writeValueBuf(Buffer *b, const char *sign, any_t val){
	switch((enum SignCh)(*sign++)){
	case pointer:{
		bool_t ok = val != NULL;
		writeBoolBuf(b, ok);
		if(ok){
			writeValueBuf(b, sign, cast_any(val, any_t));
		}
		return;
	}
	case array:{
		slice_t sli = cast_any(val, slice_t);
		writeUint32Buf(b, (uint32_t)(sli.size));
		size_t elem = get_size((enum SignCh)(*sign));
		for(size_t i = 0; i < sli.size; ++i){
			writeValueBuf(b, sign, sli.p + i * elem);
		}
		return;
	}
	case string:
		writeStringBuf(b, cast_any(val, char*));
		return;
	case bool:
		writeBoolBuf(b, cast_any(val, bool_t));
		return;
	case uint8:
	case int8:
		writeUint8Buf(b, cast_any(val, uint8_t));
		return;
	case uint16:
	case int16:
		writeUint16Buf(b, cast_any(val, uint16_t));
		return;
	case uint32:
	case int32:
	case float32:
		writeUint32Buf(b, cast_any(val, uint32_t));
		return;
	case uint64:
	case int64:
	case float64:
		writeUint64Buf(b, cast_any(val, uint64_t));
		return;
	}
}

void _skipSign(const char **sign){
	while(**sign){
		switch((enum SignCh)(*(*sign)++)){
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
			return;
		}
	}
}

void readValue0(Buffer *b, const char **sign, any_t val){
	switch((enum SignCh)(*(*sign)++)){
	case pointer:{
		bool_t ok = readBoolBuf(b);
		if(!ok){
			*(any_t*)(val) = NULL;
			_skipSign(sign);
		}else{
			readValue0(b, sign, *(any_t*)(val) = malloc(get_size((enum SignCh)(**sign))));
		}
		return;
	}
	case array:{
		uint32_t size = readUint32Buf(b);
		size_t elem = get_size((enum SignCh)(*(*sign)++));
		slice_t sli = _makeSlice(elem, size, size);
		*(slice_t *)(val) = sli;
		for(size_t i = 0; i < size; ++i){
			readValue0(b, sign, sli.p + i * elem);
		}
		return;
	}
	case string:{
		readStringBuf(b, (const char**)(val));
		return;
	}
	case bool:
		*(bool_t*)(val) = readBoolBuf(b);
		return;
	case uint8:
	case int8:
		*(uint8_t*)(val) = readUint8Buf(b);
		return;
	case uint16:
	case int16:
		*(uint16_t*)(val) = readUint16Buf(b);
		return;
	case uint32:
	case int32:
	case float32:
		*(uint32_t*)(val) = readUint32Buf(b);
		return;
	case uint64:
	case int64:
	case float64:
		*(uint64_t*)(val) = readUint64Buf(b);
		return;
	}
	return;
}

void readValue(Buffer *b, const char *sign, any_t val){
	readValue0(b, &sign, val);
}

void *readValue1(Buffer *b, const char *sign, slice_t *ptrs){
	void *args = NULL;
	void *t;
	const char *st;
	size_t size;
	size_t off = 0;
	while(*sign && (size = get_size((enum SignCh)(*sign)))){
		st = sign;
		args = realloc(args, off += size);
		readValue0(b, &sign, t = (args + off - size));
		switch((enum SignCh)(*st)){
		case pointer:
			if(cast_any(t, any_t) == NULL){
				printf("nil ptr\n");
				break;
			}
		case array:{
			signed_ptr sp = {
				.s = st,
				.v = (any_t)(off - size),
			};
			slice_append(*ptrs, sp, signed_ptr);
			break;
		}
		default:;
		}
	}
	for(size_t i = 0; i < ptrs->size; ++i){
		signed_ptr *sp = slice_at(*ptrs, i, signed_ptr);
		sp->v += (size_t)(args);
		switch((enum SignCh)(*sp->s)){
		case pointer:
			++(sp->s);
			sp->v = cast_any(sp->v, any_t);
		default:;
		}
	}
	return args;
}

void _freeArgsWithSign(void *val, const char **sign){
	switch((enum SignCh)(*((*sign)++))){
	case pointer:{
		if((val = cast_any(val, any_t)) != NULL){
			_freeArgsWithSign(val, sign);
			free(val);
		}else{
			_skipSign(sign);
		}
		return;
	}
	case array:{
		size_t elem = get_size((enum SignCh)(*(*sign)++));
		slice_t sli = cast_any(val, slice_t);
		for(size_t i = 0; i < sli.size; ++i){
			_freeArgsWithSign(sli.p + i * elem, sign);
		}
		clean_slice(sli);
		return;
	}
	case string:
		free(cast_any(val, char*));
		break;
	default:;
	}
}

void freeArgsWithSign(void *args, const char *sign){
	size_t size;
	size_t off = 0;
	while(*sign && (size = get_size((enum SignCh)(*sign)))){
		_freeArgsWithSign(args + (off += size) - size, &sign);
	}
	free(args);
}
