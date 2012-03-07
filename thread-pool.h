/**
 * thread-pool.h
 *
 * This file declares the functionality associated with
 * your implementation of a thread pool.
 */

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

// maximum number of threads allowed in a pool
#define MAX_NUM_THREADS 200

// Define TPThreadPoolT as a forward reference to an underfined
// struct TPThreadPoolT_.  You need to define this struct in your
// implementation.
typedef struct TPThreadPoolT_* TPThreadPoolT;

// TPWorkFuncT is the signature required for any function
// that is to be executed by a thread in the thread pool as
// "a piece of work"
typedef void (*TPWorkFuncT)(void *);

/*
 * Create a thread pool with numThreads threads, and a work queue
 * that can hold up to maxWorkQSize outstanding work items.  Return
 * a newly create thread pool object if successful, NULL otherwise.
 */
TPThreadPoolT TPCreateThreadPool(int numThreads, int maxWorkQSize);

/*
 * Add a work item to a the work queue of a thread pool.
 * The work item is a pointer to a function and the argument
 * that should be passed to that function when it is invoked.
 *
 * If the work queue is full, that is, it already contains
 * maxWorkQSize items, then TPAddWork should block until
 * there is a free slot in the queue.
 */
void TPAddWork(TPThreadPoolT tp, TPWorkFuncT fp, void *arg);

/*
 * Kill all threads in a thread pool and deallocate the object.
 */
void TPDestroyThreadPool(TPThreadPoolT tp);

#endif /* _THREAD_POOL_H_ */
