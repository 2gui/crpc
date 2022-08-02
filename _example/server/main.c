
#include <stdio.h>

#if 1
#include "typedef.h"
#include "buffer.h"
#include "encoding.h"
#include "sign.h"
#include "command.h"
#include "context.h"
#include "point.h"
#else
#include <Crpc.h>
#endif

int helloWorld(rpc_context *ctx){
	printf("returning hello world\n");
	rpc_return_string(ctx, "hello world");
	return 0;
}

int add(rpc_context *ctx){
	int32_t a, b;
	rva_list vl = ctx->args;
	a = rva_arg(vl, int32_t);
	b = rva_arg(vl, int32_t);
	printf("adding: %d + %d\n", a, b);
	int32_t c = a + b;
	rpc_return_int32(ctx, c);
	return 0;
}

int add_ptr(rpc_context *ctx){
	int32_t a, b, *c;
	rva_list vl = ctx->args;
	a = rva_arg(vl, int32_t);
	b = rva_arg(vl, int32_t);
	c = rva_arg(vl, int32_t*);
	printf("ctx->args %p\n", ctx->args);
	printf("adding ptr: %d + %d, %p\n", a, b, c);
	*c = a + b;
	rpc_return_null(ctx);
	return 0;
}

int div_ptr(rpc_context *ctx){
	int32_t a, b, **c;
	rva_list vl = ctx->args;
	a = rva_arg(vl, int32_t);
	b = rva_arg(vl, int32_t);
	c = rva_arg(vl, int32_t**);
	printf("diving ptr: %d + %d, %p\n", a, b, c);
	if(b == 0){
		*c = NULL;
	}else{
		**c = a / b;
	}
	rpc_return_null(ctx);
	return 0;
}

int error_test(rpc_context *ctx){
	slice_t args;
	rva_list vl = ctx->args;
	args = rva_arg(vl, slice_t);
	printf("erroring: %zu\n", args.size);
	rpc_error(ctx, "test error");
	return 0;
}

int main(int argc, const char *argv[]){
	point_t *p = newPoint(fdopen(3, "r"), fdopen(4, "w"));
	pointRegisterMethod(p, "helloWorld", "", helloWorld);
	pointRegisterMethod(p, "add", "ll", add);
	pointRegisterMethod(p, "add_ptr", "ll*l", add_ptr);
	pointRegisterMethod(p, "div_ptr", "ll**l", div_ptr);
	pointRegisterMethod(p, "error", "[i", error_test);
	printf("start listening\n");
	pointListen(p);
}
