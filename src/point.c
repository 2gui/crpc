
#include <stdio.h>

#include <typedef.h>
#include <encoding.h>
#include <buffer.h>
#include <command.h>
#include "point.h"

point_t *newPoint(FILE *r, FILE *w){
	point_t *p = malloc(sizeof(point_t)), _p = {
		.r = r,
		.w = w,
		.flag = FALSE,
		.funcs = malloc(sizeof(slice_t)),
		.cmds = {NULL},
	};
	*p = _p;
	*p->funcs = makeSlice(rpc_callback, 0, 8);
	return p;
}

int pointRegisterMethod(point_t *p, const char *name, const char *sign, rpc_callback cb){
	uint32_t id = p->funcs->size;
	func_t func = {
		.sign = sign,
		.cb = cb,
	};
	slice_append(p->funcs, func, func_t);
	DefCmd dc = {
		.name = name,
		.id = id,
	};
	return pointSendCommand(p, CmdDef, wrapDefCmd(&dc));
}

int pointSendCommand(point_t *p, uint8_t id, cmd_interface cmd){
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
	uint8_t id;
	size_t n;
	while(1){
		n = readUint32(p->r, &l);
		if(n == 0){
			return 0;
		}
		readUint8(p->r, &id);
		CmdParser cp = p->cmds[id];
		cp(p->r, p);
	}
}
