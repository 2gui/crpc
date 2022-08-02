
#include <stdio.h>

#include "platform.h"
#include "typedef.h"
#include "encoding.h"
#include "buffer.h"
#include "sign.h"
#include "command.h"
#include "thread.h"
#include "point.h"

point_t *newPoint(FILE *r, FILE *w){
	setvbuf(r, NULL, _IONBF, 0);
	setvbuf(w, NULL, _IONBF, 0);
	point_t *p = malloc(sizeof(point_t)), _p = {
		.r = r,
		.w = w,
		.flag = FALSE,
		.funcs = makeSlice(rpc_callback, 0, 8),
		.cmds = {0},
	};
	*p = _p;
	p->cmds[CmdPing] = wrapPingCmd(NULL);
	p->cmds[CmdPong] = wrapPongCmd(NULL);
	p->cmds[CmdDef] = wrapDefCmd(NULL);
	p->cmds[CmdCall] = wrapCallCmd(NULL);
	p->cmds[CmdReturn] = wrapReturnCmd(NULL);
	p->cmds[CmdError] = wrapErrorCmd(NULL);
	return p;
}

int pointRegisterMethod(point_t *p, const char *name, const char *sign, rpc_callback cb){
	uint32_t id = p->funcs.size;
	func_t func = {
		.sign = sign,
		.count = signCount(sign),
		.cb = cb,
	};
	assert(!*sign || func.count != 0, "Unexpect method sign '%s'", sign);
	debugf("registed method: %s(%s)", name, sign);
	slice_append(p->funcs, func, func_t);
	DefCmd dc = {
		.name = name,
		.id = id,
	};
	return pointSendCommand(p, CmdDef, wrapDefCmd(&dc));
}

int pointSendCommand(point_t *p, CmdID id, cmd_interface cmd){
	Buffer _buf = makeBuffer(0, 256);
	Buffer *buf = &_buf;
	writeUint8Buf(buf, id);
	cmd.m->writeTo(cmd.p, buf);
	writeUint32(p->w, (uint32_t)(buf->size));
	writeBufTo(buf, p->w);
	return 0;
}

int pointListen(point_t *p){
	uint32_t l;
	CmdID id;
	size_t n;
#ifdef DEBUG
	bool_t dflg = TRUE;
#endif
	debugf("making buffer");
	Buffer b = makeBuffer(32767, 32767);
	while(1){
		debugf("reading l");
		n = readUint32(p->r, &l);
		if(n == 0){
#ifdef DEBUG
			if(dflg){
				dflg = FALSE;
				continue;
			}
#endif
			return 0;
		}
		debugf("read l is %d", l);
		readBufFrom(&b, p->r, (size_t)(l));
		id = readUint8Buf(&b);
		cmd_interface cmd = p->cmds[id];
		if(cmd.m != NULL){
			debugf("start parsing command %02x %d", id, l);
			cmd.m->readFrom(&b, p);
			debugf("ended");
		}
	}
	freeBuffer(b);
}

queue *point_get_thread(point_t *p, uint16_t tid){
	assert(FALSE, "TODO");
	return NULL;
}

void point_stop_thread(point_t *p, uint16_t tid){
	assert(FALSE, "TODO");
	queue *que = point_get_thread(p, tid);
	if(que != NULL){
		//
	}
}
