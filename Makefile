CC = gcc
CFLAGS = -Wall -Wextra -std=c11

sim: main.o algorithm.o common.o list.o process.o
	$(CC) $(CFLAGS) -o sim main.o algorithm.o common.o list.o process.o

main.o: main.c algorithm.h
	$(CC) $(CFLAGS) -c main.c

algorithm.o: algorithm.c algorithm.h process.h
	$(CC) $(CFLAGS) -c algorithm.c

common.o: common.c common.h
	$(CC) $(CFLAGS) -c common.c

list.o: list.c list.h common.h
	$(CC) $(CFLAGS) -c list.c

process.o: process.c process.h list.h common.h
	$(CC) $(CFLAGS) -c process.c

clean:
	rm -f *.o sim
