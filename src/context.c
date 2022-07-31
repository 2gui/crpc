
#include <stdio.h>
#include <stdlib.h>

#include "platform.h"
#include "typedef.h"
#include "command.h"
#include "encoding.h"
#include "sign.h"
#include "point.h"
#include "thread.h"
#include "context.h"

const uint32_t SesCall = (1 << 16) - 1;
const uint32_t SesThread = ~SesCall;

int readFuncContext(Buffer *r, rpc_context *ctx){
	size_t al = (size_t)(readUint16Buf(r));
	if(al != ctx->func->count){
		fprintf(stderr, "error: arguments length not same, expect '%s'(%lu) but got %zu\n", ctx->func->sign, ctx->func->count, al);
		exit(1);
		return 1;
	}
	ctx->flag = FALSE;
	if(*ctx->func->sign){
		ctx->ptrs = makeSlice(signed_ptr, 0, al / 2 + 1);
		ctx->args = (rva_list)(readValue1(r, ctx->func->sign, &ctx->ptrs));
	}else{
		debugf("set ctx->ptrs nil");
		ctx->ptrs.size = 0;
		ctx->ptrs.cap = 0;
		ctx->args = NULL;
	}
	return 0;
}

int executeContext(rpc_context *ctx){
	int code;
	uint16_t tid = (uint16_t)(ctx->sesid >> 16);
	if(tid){
		assert(FALSE, "TODO");
		queue *que = point_get_thread(ctx->p, tid);
		if(que == NULL){
			return 4;
		}
		queue_put(que, ctx);
	}else{
		debugf("callbacking");
		code = ctx->func->cb(ctx);
		debugf("called, cleaning");
		cleanContext(ctx);
		debugf("freeing ctx");
		free(ctx);
	}
	return code;
}

int cleanContext(rpc_context *ctx){
	debugf("clean ctx ptrs %p", ctx->ptrs.p);
	clean_slice(ctx->ptrs);
	debugf("cleaning ctx args");
	if(ctx->args != NULL){
		freeArgsWithSign(ctx->args, ctx->func->sign);
	}
	return 0;
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

int rpc_return_bool(rpc_context *ctx, bool_t val){
	return rpc_return_c(ctx, "A", &val);
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
