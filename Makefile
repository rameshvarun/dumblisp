CC=gcc

.PHONY: default test clean

default: dumblisp

dumblisp: lexer.o interpreter.o
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~

test:
	./runtests