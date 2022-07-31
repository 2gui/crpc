
#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

typedef struct{
	pthread_mutex_t mux;
	pthread_cond_t cond;
	struct queue_item{
		void *val;
		struct queue_item *next;
	} *head, *tail;
} queue;

queue makeQueue();
void *queue_get(queue *que);
void queue_put(queue *que, void *val);
void queue_clear(queue *que);


#endif // __THREAD_H__
