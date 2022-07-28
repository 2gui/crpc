
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
	writeBoolBuf(b, p->val != NULL);
	if(p->val != NULL){
		putStringBuf(b, p->sign);
		writeValueBuf(b, p->sign, p->val);
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
	uint16_t l;
	readUint16(r, &l);
	size_t c = signCount(func.sign);
	if(l != c){
		fprintf(stderr, "error: arguments length not same, except %lu but got %d\n", c, l);
		exit(-1);
		return -1;
	}
	rpc_context ctx = {
		.args = NULL,
		.sesid = cc.sesid,
		.flag = FALSE,
		.p = p,
	};
	if(*func.sign){
		ctx.args = (rva_list)(readValue1(r, func.sign));
	}
	func.cb(&ctx);
	return 0;
}
