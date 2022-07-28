
#include <stdio.h>

#include <Crpc.h>

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
	rva_end(vl, ctx->args);
	printf("adding: %d + %d\n", a, b);
	int32_t c = a + b;
	rpc_return_int32(ctx, c);
	return 0;
}

int error_test(rpc_context *ctx){
	slice_t args;
	rva_list vl = ctx->args;
	args = rva_arg(vl, slice_t);
	rva_end(vl, ctx->args);
	printf("erroring: %zu\n", args.size);
	rpc_error_c(ctx, "test error");
	return 0;
}

int main(int argc, const char *argv[]){
	point_t *p = newPoint(fdopen(3, "r"), fdopen(4, "w"));
	pointRegisterMethod(p, "helloWorld", "", helloWorld);
	pointRegisterMethod(p, "add", "ll", add);
	pointRegisterMethod(p, "error", "[i", error_test);
	printf("start listening\n");
	pointListen(p);
}
