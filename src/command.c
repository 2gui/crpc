
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "encoding.h"
#include "buffer.h"
#include "sign.h"
#include "point.h"
#include "command.h"

const CmdID CmdPing   = 0x00;
const CmdID CmdPong   = 0x01;
const CmdID CmdDef    = 0x02;
const CmdID CmdCall   = 0x03;
const CmdID CmdReturn = 0x04;
const CmdID CmdError  = 0x05;

const ErrID ErrString    = 0x00l << 32;
const ErrID ErrNotExists = 0x01l << 32;
const ErrID ErrArgs      = 0x02l << 32;
const ErrID ErrCustom    = 0xffl << 32;
const ErrID ErrIdMask     = 0xffffffffl << 32;
const ErrID ErrCustomMask = ~ErrIdMask;

#define _commandIWrapper(Type) cmd_interface wrap##Type(Type *p){\
	static const cmd_methods_t _##Type##M = {\
		.writeTo = Type##_writeTo,\
		.readFrom = Type##_readFrom\
	};\
	cmd_interface v = {\
		.p = p,\
		.m = &_##Type##M,\
	};\
	return v;\
}

int PingCmd_writeTo(void *_p, Buffer *b){
	PingCmd *p = _p;
	writeUint64Buf(b, p->d);
	return 0;
}

int PingCmd_readFrom(Buffer *b, point_t *p){
	PongCmd c;
	c.d = readUint64Buf(b);
	return pointSendCommand(p, CmdPong, wrapPongCmd(&c));
}

_commandIWrapper(PingCmd)

int PongCmd_writeTo(void *_p, Buffer *b){
	PongCmd *p = _p;
	writeUint64Buf(b, p->d);
	return 0;
}

int PongCmd_readFrom(Buffer *b, point_t *p){
	PongCmd c;
	c.d = readUint64Buf(b);
	// TODO
	return 0;
}

_commandIWrapper(PongCmd)

int DefCmd_writeTo(void *_p, Buffer *b){
	DefCmd *p = _p;
	writeUint32Buf(b, p->id);
	writeStringBuf(b, p->name);
	return 0;
}

int DefCmd_readFrom(Buffer *b, point_t *t){
	DefCmd c;
	c.id = readUint32Buf(b);
	readStringBuf(b, &c.name);
	// TODO
	return 0;
}

_commandIWrapper(DefCmd)

int CallCmd_writeTo(void *_p, Buffer *b){
	CallCmd *p = _p;
	assert(FALSE, "TODO");
	return 0;
}

int CallCmd_readFrom(Buffer *b, point_t *p){
	CallCmd c;
	c.id = readUint32Buf(b);
	if(c.id >= p->funcs.size){
		fprintf(stderr, "error: method id '%d' out of range '%zu'\n", c.id, p->funcs.size);
		exit(-1);
		return -1;
	}
	rpc_context *ctx = malloc(sizeof(rpc_context));
	ctx->func = slice_at(p->funcs, c.id, func_t);
	ctx->sesid = readUint32Buf(b);
	ctx->p = p;
	debugf("calling %d method", c.id);
	int n = readFuncContext(b, ctx);
	if(n){
		return n;
	}
	return executeContext(ctx);
}

_commandIWrapper(CallCmd)

int ReturnCmd_writeTo(void *_p, Buffer *b){
	ReturnCmd *p = _p;
	writeUint32Buf(b, p->sesid);
	bool_t ok = *p->sign != 0;
	writeBoolBuf(b, ok);
	if(ok){
		putStringBuf(b, p->sign);
		writeValueBuf(b, p->sign, p->val);
	}
	writeUint16Buf(b, p->ptrs.size);
	signed_ptr *sp;
	for(size_t i = 0; i < p->ptrs.size; ++i){
		sp = slice_at(p->ptrs, i, signed_ptr);
		writeValueBuf(b, sp->s, sp->v);
	}
	return 0;
}

int ReturnCmd_readFrom(Buffer *b, point_t *p){
	assert(FALSE, "TODO");
	return 0;
}

_commandIWrapper(ReturnCmd)

int ErrorCmd_writeTo(void *_p, Buffer *b){
	ErrorCmd *p = _p;
	writeUint32Buf(b, p->sesid);
	writeUint64Buf(b, p->errid);
	writeStringBuf(b, p->err);
	writeUint16Buf(b, p->ptrs.size);
	signed_ptr sp;
	for(size_t i = 0; i < p->ptrs.size; ++i){
		sp = slice_get(p->ptrs, i, signed_ptr);
		writeValueBuf(b, sp.s, sp.v);
	}
	return 0;
}

int ErrorCmd_readFrom(Buffer *b, point_t *p){
	assert(FALSE, "TODO ErrorCmd.readFrom");
	ErrorCmd c;
	c.sesid = readUint32Buf(b);
	c.errid = readUint64Buf(b);
	readStringBuf(b, &c.err);
	free((void*)(c.err));
	return 0;
}

_commandIWrapper(ErrorCmd)
