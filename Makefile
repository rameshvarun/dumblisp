CC=gcc
LIBS=-lreadline
CFLAGS=-std=c99

.PHONY: default test clean

default: dumblisp

OBJS=src/lexer.o src/interpreter.o src/parser.o src/emitter.o src/scope.o src/expression.o src/builtins.o

dumblisp: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

src/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete

test:
	./runtests.sh

format:
	clang-format -i *.c *.h
