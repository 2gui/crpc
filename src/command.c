
#include <string.h>
#include <stdlib.h>

#include "encoding.h"
#include "buffer.h"
#include "sign.h"
#include "point.h"
#include "command.h"

const uint8_t CmdPing   = 0x00;
const uint8_t CmdPong   = 0x01;
const uint8_t CmdDef    = 0x02;
const uint8_t CmdCall   = 0x03;
const uint8_t CmdReturn = 0x04;
const uint8_t CmdError  = 0x05;

int PongCmd_writeTo(void *_p, Buffer *b){
	PongCmd *p = (PongCmd*)(_p);
	writeUint64Buf(b, p->d);
	return 0;
}

cmd_interface wrapPongCmd(PongCmd *p){
	static const cmd_methods_t _PongCmdM = {
		.writeTo = PongCmd_writeTo,
	};

	cmd_interface v = {
		.p = p,
		.m = &_PongCmdM,
	};
	return v;
}

int DefCmd_writeTo(void *_p, Buffer *b){
	DefCmd *p = (DefCmd*)(_p);
	writeUint32Buf(b, p->id);
	writeStringBuf(b, p->name);
	return 0;
}

cmd_interface wrapDefCmd(DefCmd *p){
	static const cmd_methods_t _DefCmdM = {
		.writeTo = DefCmd_writeTo,
	};

	cmd_interface v = {
		.p = p,
		.m = &_DefCmdM,
	};
	return v;
}

int ReturnCmd_writeTo(void *_p, Buffer *b){
	ReturnCmd *p = (ReturnCmd*)(_p);
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

cmd_interface wrapReturnCmd(ReturnCmd *p){
	static const cmd_methods_t _ReturnCmdM = {
		.writeTo = ReturnCmd_writeTo,
	};

	cmd_interface v = {
		.p = p,
		.m = &_ReturnCmdM,
	};
	return v;
}

const uint16_t ErrString    = 0x00;
const uint16_t ErrNotExists = 0x01;
const uint16_t ErrArgs      = 0x02;

int ErrorCmd_writeTo(void *_p, Buffer *b){
	ErrorCmd *p = (ErrorCmd*)(_p);
	writeUint32Buf(b, p->sesid);
	writeUint16Buf(b, p->errid);
	writeStringBuf(b, p->err);
	writeUint16Buf(b, p->ptrs.size);
	signed_ptr sp;
	for(size_t i = 0; i < p->ptrs.size; ++i){
		sp = slice_get(p->ptrs, i, signed_ptr);
		writeValueBuf(b, sp.s, sp.v);
	}
	return 0;
}

cmd_interface wrapErrorCmd(ErrorCmd *p){
	static const cmd_methods_t _ErrorCmdM = {
		.writeTo = ErrorCmd_writeTo,
	};

	cmd_interface v = {
		.p = p,
		.m = &_ErrorCmdM,
	};
	return v;
}

int parsePingCmd(FILE *r, point_t *p){
	PongCmd pc;
	readUint64(r, &pc.d);
	return pointSendCommand(p, CmdPong, wrapPongCmd(&pc));
}

int parseCallCmd(FILE *r, point_t *p){
	CallCmd cc;
	readUint32(r, &cc.id);
	readUint32(r, &cc.sesid);
	func_t func = slice_get(p->funcs, cc.id, func_t);
	int code = callFuncContext(r, cc.sesid, func, p);
	return code;
}
