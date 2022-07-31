
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <stdio.h>

#include "buffer.h"

const uint8_t CmdPing;
const uint8_t CmdPong;
const uint8_t CmdDef;
const uint8_t CmdCall;
const uint8_t CmdReturn;
const uint8_t CmdError;

struct point_t;

typedef struct{
	int (*writeTo)(void *p, Buffer *w);
	int (*readFrom)(Buffer *b, struct point_t *p);
} cmd_methods_t;

typedef struct{
	void *p;
	const cmd_methods_t* m;
} cmd_interface;


typedef struct{
	uint64_t d;
} PingCmd;
cmd_interface wrapPingCmd(PingCmd *p);

typedef struct{
	uint64_t d;
} PongCmd;
cmd_interface wrapPongCmd(PongCmd *p);

typedef struct{
	const char *name;
	uint32_t id;
} DefCmd;
cmd_interface wrapDefCmd(DefCmd *p);

typedef struct{
	uint32_t id;
	uint32_t sesid;
	void *args;
} CallCmd;
cmd_interface wrapCallCmd(CallCmd *p);

typedef struct{
	uint32_t sesid;
	const char *sign;
	any_t val;
	slice_t ptrs;
} ReturnCmd;
cmd_interface wrapReturnCmd(ReturnCmd *p);

const uint16_t ErrString;
const uint16_t ErrNotExists;
const uint16_t ErrArgs;

typedef struct{
	uint32_t sesid;
	uint16_t errid;
	const char *err;
	slice_t ptrs;
} ErrorCmd;
cmd_interface wrapErrorCmd(ErrorCmd *p);

#endif // __COMMAND_H__
