
#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef char bool_t;

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long long int int64_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef float float32_t;
typedef double float64_t;

typedef void *any_t;
#define cast_any(v, t) (*(t*)((any_t)(v)))

typedef struct{
	size_t size;
	size_t cap;
	void *p;
} slice_t;

slice_t _makeSlice(size_t elem, size_t size, size_t cap);
#define makeSlice(t, size, cap) _makeSlice(sizeof(t), size, cap)
#define slice_at(s, i, t) ((t*)(s).p + (i))
#define slice_get(s, i, t) (*slice_at(s, i, t))
#define slice_set(s, i, e, t) (*slice_at(s, i, t) = e)
#define slice_append(s, e, t) do{ \
	if((s).size == (s).cap){        \
		if((s).cap < 1024){           \
			(s).cap = (s).cap / 2 * 3 + 1;\
		}else{                        \
			(s).cap += 1024;            \
		}                             \
		(s).p = realloc((s).p, (s).cap * sizeof(t));\
	}                               \
	slice_set(s, (s).size, e, t);   \
	++((s).size);                   \
}while(0)
#define free_slice(s) do{ if((s).p != NULL){ free((s).p); (s).p = NULL; } }while(0)

#endif // __TYPE_DEF_H__
