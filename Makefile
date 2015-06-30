CC=gcc
LIBS=-lreadline

.PHONY: default test clean

default: dumblisp

dumblisp: lexer.o interpreter.o parser.o
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~

test:
	./runtests

format:
	clang-format -i *.c *.h
