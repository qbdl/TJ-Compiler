CC=gcc
CFLAGS=-std=c11 -g -fno-common
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

chibicc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS):chibi.h

test: chibicc
	./chibicc tests > tmp.s
	gcc -static -o tmp tmp.s
	./tmp

clean:
	rm -f chibicc *.o *~ tmp*

.PHONY: test clean