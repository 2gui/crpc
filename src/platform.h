
#ifndef __PlATFORM_H__
#define __PlATFORM_H__

#include <stdio.h>
#include <stdlib.h>

/* #undef DEBUG */

#ifdef DEBUG
#define debugf(format, ...) fprintf(stderr, "DEBUG: " format "\n", ##__VA_ARGS__)
#else
#define debugf(...)
#endif

#define assert(expr, format, ...) do{\
	if(!(expr)){\
		fprintf(stderr, "%s:%d: ASSERT ERROR: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__);\
		exit(-1);\
	}\
}while(0)

#endif // __PlATFORM_H__
