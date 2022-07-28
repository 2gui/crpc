
#include <typedef.h>
#include <command.h>
#include <point.h>
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

int rpc_error_c(rpc_context *ctx, const char *err){
	if(ctx->flag){
		return 1;
	}
	ctx->flag = TRUE;
	ErrorCmd ec = {
		.sesid = ctx->sesid,
		.err = err,
	};
	return pointSendCommand(ctx->p, CmdError, wrapErrorCmd(&ec));
}
