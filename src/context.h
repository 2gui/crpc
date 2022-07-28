
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdio.h>
#include <stdlib.h>

#include <typedef.h>

struct point_t;

typedef void *rva_list;

typedef struct{
	rva_list args;
	uint32_t sesid;
	bool_t flag;
	struct point_t *p;
} rpc_context;

#define rva_arg(l, t) (*(t*)((l += sizeof(t)) - sizeof(t)))
#define rva_end(l, a) do{ l = NULL; if(a != NULL){ free(a); a = NULL; } }while(0)

typedef int (*rpc_callback)(rpc_context*);

int rpc_return_c(rpc_context *ctx, const char *sign, const any_t val);
int rpc_error_c(rpc_context *ctx, const char *err);

#endif // __CONTEXT_H__
