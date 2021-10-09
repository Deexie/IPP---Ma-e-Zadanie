CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

.PHONY: all clean

all: forests

forests: main.o input.o commands.o avl.o
	$(CC) $(LDFLAGS) -o $@ $^

avl.o: avl.c avl.h
	$(CC) $(CFLAGS) -c $<

commands.o: commands.c commands.h avl.h
	$(CC) $(CFLAGS) -c $<

input.o: input.c input.h commands.h avl.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c input.h commands.h avl.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o forests
