
#ifndef __POINT_H__
#define __POINT_H__

#include <stdio.h>

#include <typedef.h>
#include <buffer.h>
#include <context.h>
#include <command.h>

typedef struct{
	const char *sign;
	const char *retsign;
	rpc_callback cb;
} func_t;

typedef int (*CmdParser)(FILE *r, struct point_t *p);

typedef struct point_t{
	FILE *r, *w;
	bool_t flag;
	slice_t *funcs;
	CmdParser cmds[256];
} point_t;

point_t *newPoint(FILE *r, FILE *w);
int pointRegisterMethod(point_t *p, const char *name, const char *sign, rpc_callback cb);
int pointSendCommand(point_t *p, uint8_t id, cmd_interface cmd);
int pointListen(point_t *p);

#endif // __POINT_H__
