CC = gcc
CCFLAGS = -Wall -pedantic -g

all: threadpool-test

threadpool-test: threadpool-test.o thread-pool.o Queue.o
	$(CC) $(CCFLAGS) -o threadpool threadpool-test.o thread-pool.o Queue.o -lpthread

threadpool-test.o: threadpool-test.c thread-pool.h
	$(CC) $(CCFLAGS) -c threadpool-test.c

thread-pool.o: thread-pool.c thread-pool.h Queue.h
	$(CC) $(CCFLAGS) -c thread-pool.c

Queue.o: Queue.c Queue.c
	$(CC) $(CCFLAGS) -c Queue.c

clean:
	rm -f threadpool *.o
