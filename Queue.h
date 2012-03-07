#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <pthread.h>
#include <stdlib.h>

#define Q_SUCCESS 1
#define Q_EMPTY 0

typedef struct Queue_* Queue; 

Queue QCreate(int size);

int enqueue(Queue qT, void* item);

int dequeue(Queue qT, void** store);

void QDestroy(Queue qT);

void QSignalAll(Queue qT);

#endif
