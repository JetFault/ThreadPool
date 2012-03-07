#include "thread-pool.h"
#include "Queue.h"

struct TPThreadPoolT_ {
	Queue q;
	pthread_t * threads;
	int isAlive;
	int threadsAlive;
	pthread_mutex_t lock;
};

struct Work_ {
	TPWorkFuncT func;
	void* arg;
}; typedef struct Work_* Work;

void *DoWork_(void* arg) {
	TPThreadPoolT tp = (TPThreadPoolT) arg;
	Work work;
	int threadsActive;

	while(1) {
		int val = dequeue(tp->q, (void*)&work);
		if(val == Q_EMPTY) {
			break;
		}
		work->func(work->arg);
	}

	pthread_mutex_lock(&(tp->lock));
	threadsActive = --(tp->threadsAlive);
	pthread_mutex_unlock(&(tp->lock));

	if(threadsActive == 0) {
		QDestroy(tp->q);
		TPDestroyThreadPool(tp);
	}
	return;
}

TPThreadPoolT TPCreateThreadPool(int numThreads, int maxWorkQSize) {
	int i;
	TPThreadPoolT tp = (TPThreadPoolT) malloc(sizeof(struct TPThreadPoolT_));
	if(tp == NULL) { return NULL; }

	tp->q = QCreate(maxWorkQSize);
	if(tp->q == NULL) { return NULL;}
	tp->isAlive = 1;
	tp->threadsAlive = 0;
	pthread_mutex_init(&(tp->lock), NULL);

	tp->threads = (pthread_t *) malloc(sizeof(pthread_t)*numThreads);
	for(i = 0; i < numThreads; i++) {
		tp->threadsAlive++;
		pthread_create(&(tp->threads[i]), NULL, DoWork_, (void*) tp);
	}
	return tp;
}

void TPAddWork(TPThreadPoolT tp, TPWorkFuncT fp, void *arg) {
	Work work = (Work) malloc(sizeof(struct Work_));
	if(work == NULL) { return; }

	work->func = fp;
	work->arg = arg;

	enqueue(tp->q, (void*)work);
}

void TPDestroyThreadPool(TPThreadPoolT tp) {
	tp->isAlive = 0;
	QSignalAll(tp->q);
}
