CC = gcc
CFLAGS = -O1 -lm

.PHONY: clean
.PHONY: r

all: a

a: main.c
	$(CC) main.c fakes.c $(CFLAGS) 
	./a.out

debug:
	$(CC) main.c fakes.c -lm -g 
	gdb ./a.out

r: a.out
	./a.out

clean:
	rm a.out
