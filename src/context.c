
#include "typedef.h"
#include "command.h"
#include "point.h"
#include "context.h"

int rpc_return_c(rpc_context *ctx, const char *sign, const any_t val){
	if(ctx->flag){
		return 1;
	}
	ctx->flag = TRUE;
	ReturnCmd rc = {
		.sesid = ctx->sesid,
		.sign = sign,
		.val = val,
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
