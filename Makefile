CC=gcc
LIBS=-lreadline -lgc
CFLAGS=-std=gnu99
DEBUGFLAGS=-ggdb -g3 -gdwarf-2
RELEASEFLAGS=-O3

.PHONY: default test clean

default: dumblisp

OBJS=src/lexer.o src/interpreter.o src/parser.o src/emitter.o src/scope.o src/expression.o src/builtins.o src/stringpool.o

dumblisp: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

src/lib.h: src/lib.lisp
	xxd -i $< | sed "s/}\;/,0x00}\;/" > $@

src/%.o: src/%.c src/lib.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	rm src/lib.h
	rm dumblisp

test:
	./runtests.sh

format:
	clang-format -i src/*.c src/*.h
