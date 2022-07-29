
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdio.h>

#include "typedef.h"

struct point_t;

typedef void *rva_list;
#define rva_arg(l, t) (*(t*)((l += sizeof(t)) - sizeof(t)))

typedef struct{
	rva_list args;
	uint32_t sesid;
	bool_t flag;
	struct point_t *p;
	slice_t ptrs;
} rpc_context;

typedef int (*rpc_callback)(rpc_context*);

typedef struct{
	const char *sign;
	const char *retsign;
	rpc_callback cb;
} func_t;

int callFuncContext(FILE *r, uint32_t sesid, func_t func, struct point_t *p);

int rpc_return_c(rpc_context *ctx, const char *sign, const any_t val);
#define rpc_return_null(ctx) rpc_return_c(ctx, "", NULL)
int rpc_return_int8(rpc_context *ctx, int8_t val);
int rpc_return_int16(rpc_context *ctx, int16_t val);
int rpc_return_int32(rpc_context *ctx, int32_t val);
int rpc_return_int64(rpc_context *ctx, int64_t val);
int rpc_return_uint8(rpc_context *ctx, uint8_t val);
int rpc_return_uint16(rpc_context *ctx, uint16_t val);
int rpc_return_uint32(rpc_context *ctx, uint32_t val);
int rpc_return_uint64(rpc_context *ctx, uint64_t val);
int rpc_return_float32(rpc_context *ctx, float32_t val);
int rpc_return_float64(rpc_context *ctx, float64_t val);
int rpc_return_string(rpc_context *ctx, const char *str);

int _rpc_error_c(rpc_context *ctx, uint16_t errid, const char *err);
#define rpc_error_c(ctx, err) _rpc_error_c(ctx, ErrString, err)

#endif // __CONTEXT_H__
