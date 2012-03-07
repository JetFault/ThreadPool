#include "Queue.h"

struct Queue_ {
	pthread_mutex_t lock;
	pthread_cond_t isFull;
	pthread_cond_t isEmpty;
	void** data;
	int numItems;
	int size;
	int head;
	int tail;
	int isAlive;
};

Queue QCreate(int size) {
	Queue qT;
	int res;
	if(size <= 0) { return NULL; }

	qT = (Queue) malloc(sizeof(struct Queue_));
	if(qT == NULL){ return NULL; }

	qT->size = size;
	qT->numItems = 0;
	qT->head = qT->tail = 0;
	qT->data = (void**) malloc(sizeof(void*)*size);
	if(qT->data == NULL) { return NULL; }
	res = pthread_mutex_init(&(qT->lock), NULL);
	if(res != 0) { return NULL; }
	qT->isAlive = 1;

	return qT;
}

int enqueue(Queue qT, void* item) {
	pthread_mutex_lock(&(qT->lock));
	while(qT->numItems == qT->size) {
		pthread_cond_wait(&(qT->isFull), &(qT->lock));
	}

	qT->data[qT->tail] = item;
	qT->tail = (qT->tail +1) % qT->size;
	qT->numItems += 1;

	pthread_cond_signal(&(qT->isEmpty));
	pthread_mutex_unlock(&(qT->lock));

	return Q_SUCCESS;
}

int dequeue(Queue qT, void** store) {
	pthread_mutex_lock(&(qT->lock));
	while(qT->numItems == 0) {
		if(qT->isAlive == 0) { /* ThreadPool Destroyed! */
			return Q_EMPTY;
		}
		pthread_cond_wait(&(qT->isEmpty), &(qT->lock));
	}

	*store = qT->data[qT->head];
	qT->head = (qT->head + 1) % qT->size;
	qT->numItems -= 1;

	pthread_cond_signal(&(qT->isFull));
	pthread_mutex_unlock(&(qT->lock));
	
	return Q_SUCCESS;
}

void QDestroy(Queue qT) {
	free(qT->data);
	free(qT);
}

void QSignalAll(Queue qT) {
	qT->isAlive = 0;
	pthread_cond_broadcast(&(qT->isEmpty));
	pthread_cond_broadcast(&(qT->isFull));
}
