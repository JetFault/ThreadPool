/**
 * threadpool_test.c, copyright 2001 Steve Gribble
 *
 * Just a regression test for the threadpool code.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "thread-pool.h"

extern int errno;

void DoWork(void *arg) {
  int seconds = (int) arg;

  fprintf(stdout, "  in dispatch %d\n", seconds);
  sleep(seconds);
  fprintf(stdout, "  done dispatch %d\n", seconds);
}

int main(int argc, char **argv) {
  TPThreadPoolT tp;

  tp = TPCreateThreadPool(2, 2);
  assert(tp != NULL);

  fprintf(stdout, "**main** dispatch 3\n");
  TPAddWork(tp, DoWork, (void *) 3);
  fprintf(stdout, "**main** dispatch 6\n");
  TPAddWork(tp, DoWork, (void *) 6);
  fprintf(stdout, "**main** dispatch 7\n");
  TPAddWork(tp, DoWork, (void *) 7);

  fprintf(stdout, "**main** done first\n");
  sleep(20);
  fprintf(stdout, "\n\n");

  fprintf(stdout, "**main** dispatch 3\n");
  TPAddWork(tp, DoWork, (void *) 3);
  fprintf(stdout, "**main** dispatch 6\n");
  TPAddWork(tp, DoWork, (void *) 6);
  fprintf(stdout, "**main** dispatch 7\n");
  TPAddWork(tp, DoWork, (void *) 7);

  fprintf(stdout, "**main Destroy**\n");
  TPDestroyThreadPool(tp);

  fprintf(stdout, "**main done second\n");
  sleep(20);
  exit(-1);
}

