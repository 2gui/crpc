
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "command.h"
#include "encoding.h"
#include "sign.h"
#include "point.h"
#include "context.h"

int callFuncContext(FILE *r, uint32_t sesid, func_t func, struct point_t *p){
	uint16_t al;
	readUint16(r, &al);
	size_t c = signCount(func.sign);
	if(al != c){
		fprintf(stderr, "error: arguments length not same, expect %lu but got %d\n", c, al);
		exit(-1);
		return -1;
	}
	rpc_context ctx = {
		.args = NULL,
		.sesid = sesid,
		.flag = FALSE,
		.p = p,
		.ptrs = makeSlice(signed_ptr, 0, 2),
	};
	if(*func.sign){
		ctx.args = (rva_list)(readValue1(r, func.sign, &ctx.ptrs));
	}
	int code = func.cb(&ctx);
	if(ctx.args != NULL){
		free_slice(ctx.ptrs);
		freeArgsWithSign(ctx.args, func.sign);
	}
	return code;
}

int rpc_return_c(rpc_context *ctx, const char *sign, const any_t val){
	if(ctx->flag){
		return 1;
	}
	ctx->flag = TRUE;

	ReturnCmd rc = {
		.sesid = ctx->sesid,
		.sign = sign,
		.val = val,
		.ptrs = ctx->ptrs,
	};
	return pointSendCommand(ctx->p, CmdReturn, wrapReturnCmd(&rc));
}

int rpc_return_int8(rpc_context *ctx, int8_t val){
	return rpc_return_c(ctx, "b", &val);
}

int rpc_return_int16(rpc_context *ctx, int16_t val){
	return rpc_return_c(ctx, "i", &val);
}

int rpc_return_int32(rpc_context *ctx, int32_t val){
	return rpc_return_c(ctx, "l", &val);
}

int rpc_return_int64(rpc_context *ctx, int64_t val){
	return rpc_return_c(ctx, "q", &val);
}

int rpc_return_uint8(rpc_context *ctx, uint8_t val){
	return rpc_return_c(ctx, "B", &val);
}

int rpc_return_uint16(rpc_context *ctx, uint16_t val){
	return rpc_return_c(ctx, "I", &val);
}

int rpc_return_uint32(rpc_context *ctx, uint32_t val){
	return rpc_return_c(ctx, "L", &val);
}

int rpc_return_uint64(rpc_context *ctx, uint64_t val){
	return rpc_return_c(ctx, "Q", &val);
}

int rpc_return_float32(rpc_context *ctx, float32_t val){
	return rpc_return_c(ctx, "F", &val);
}

int rpc_return_float64(rpc_context *ctx, float64_t val){
	return rpc_return_c(ctx, "D", &val);
}

int rpc_return_string(rpc_context *ctx, const char *str){
	return rpc_return_c(ctx, "S", (any_t)(str));
}

int _rpc_error_c(rpc_context *ctx, uint16_t errid, const char *err){
	if(ctx->flag){
		return 1;
	}
	ctx->flag = TRUE;
	ErrorCmd ec = {
		.sesid = ctx->sesid,
		.errid = errid,
		.err = err,
	};
	return pointSendCommand(ctx->p, CmdError, wrapErrorCmd(&ec));
}
