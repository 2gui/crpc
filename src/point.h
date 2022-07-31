
#ifndef __POINT_H__
#define __POINT_H__

#include <stdio.h>

#include "typedef.h"
#include "buffer.h"
#include "context.h"
#include "command.h"
#include "thread.h"

typedef struct point_t{
	FILE *r, *w;
	bool_t flag;
	slice_t funcs;
	cmd_interface cmds[256];
} point_t;

point_t *newPoint(FILE *r, FILE *w);
int pointRegisterMethod(point_t *p, const char *name, const char *sign, rpc_callback cb);
int pointSendCommand(point_t *p, uint8_t id, cmd_interface cmd);
int pointListen(point_t *p);
uint16_t point_new_thread(point_t *p);
queue *point_get_thread(point_t *p, uint16_t tid);
void point_stop_thread(point_t *p, uint16_t tid);

#endif // __POINT_H__
