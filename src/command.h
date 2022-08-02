
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <stdio.h>

#include "buffer.h"

#define CmdID uint8_t
#define ErrID uint64_t

const CmdID CmdPing;
const CmdID CmdPong;
const CmdID CmdDef;
const CmdID CmdCall;
const CmdID CmdReturn;
const CmdID CmdError;

const ErrID ErrString;
const ErrID ErrNotExists;
const ErrID ErrArgs;
const ErrID ErrCustom;
const ErrID ErrIdMask;
const ErrID ErrCustomMask;

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

typedef struct{
	uint32_t sesid;
	ErrID errid;
	const char *err;
	slice_t ptrs;
} ErrorCmd;
cmd_interface wrapErrorCmd(ErrorCmd *p);

#endif // __COMMAND_H__
