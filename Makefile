CC=gcc
LIBS=-lreadline
CFLAGS=-std=c99

.PHONY: default test clean

default: dumblisp

dumblisp: lexer.o interpreter.o parser.o emitter.o scope.o expression.o builtins.o
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~

test:
	./runtests.sh

format:
	clang-format -i *.c *.h
