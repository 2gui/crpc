
#include <stdio.h>

#include <context.h>
#include <point.h>

int helloWorld(rpc_context *ctx){
	rpc_return_c(ctx, "[B", "hello world");
	return 0;
}

int add(rpc_context *ctx){
	int a, b;
	rva_list vl = ctx->args;
	a = rva_arg(vl, int);
	b = rva_arg(vl, int);
	rva_end(vl, ctx->args);
	int c = a + b;
	rpc_return_c(ctx, "L", &c);
	return 0;
}

int main(int argc, const char **argv){
	printf("1\n");
	point_t *p = newPoint(fdopen(3, "r"), fdopen(4, "w"));
	printf("2\n");
	pointRegisterMethod(p, "helloWorld", "", helloWorld);
	printf("3\n");
	pointRegisterMethod(p, "add", "LL", add);
	printf("4\n");
	pointListen(p);
}
