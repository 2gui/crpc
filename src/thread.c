
#include <pthread.h>

#include "point.h"
#include "thread.h"


queue makeQueue(){
	queue que = {
		.mux = PTHREAD_MUTEX_INITIALIZER,
		.cond = PTHREAD_COND_INITIALIZER,
		.head = NULL,
		.tail = NULL,
	};
	pthread_mutex_init(&que.mux, NULL);
	pthread_cond_init(&que.cond, NULL);
	return que;
}

void *queue_get(queue *que){
	pthread_mutex_lock(&que->mux);
	if(que->head == NULL){
		pthread_cond_wait(&que->cond, &que->mux);
	}
	struct queue_item *item = que->head;
	if((que->head = item->next) == NULL){
		que->tail = NULL;
	}
	void *val = item->val;
	free(item);
	pthread_mutex_unlock(&que->mux);
	return val;
}

void queue_put(queue *que, void *val){
	pthread_mutex_lock(&que->mux);
	struct queue_item *item = malloc(sizeof(struct queue_item));
	item->val = val;
	item->next = NULL;
	if(que->tail == NULL){
		que->head = que->tail = item;
	}else{
		que->tail = (que->tail->next = item);
	}
	pthread_cond_signal(&que->cond);
	pthread_mutex_unlock(&que->mux);
}

void queue_clear(queue *que){
	pthread_mutex_lock(&que->mux);
	if(que->head != NULL){
		if(que->head == que->tail){
			free(que->head);
			que->head = NULL;
		}else{
			struct queue_item *item = que->head;
			while(item != NULL){
				que->head = item->next;
				free(item);
				item = que->head;
			}
		}
		que->tail = NULL;
	}
	pthread_mutex_unlock(&que->mux);
}

void *point_thread_listener(void *_q){
	queue *que = _q;
	point_t *p = queue_get(que);
	void *e;
	while((e = queue_get(que)) != NULL){
		;
	}
	queue_clear(que);
	free(que);
	return NULL;
}

int new_thread(point_t *p, queue *que){
	*que = makeQueue();
	queue_put(que, p);
	pthread_t thread;
	pthread_create(&thread, NULL, point_thread_listener, que);
	return 0;
}
