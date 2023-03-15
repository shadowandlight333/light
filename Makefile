CC = gcc
CFLAGS = -lrt
OBJS = servidor cliente
BIN_FILES = servidor cliente

all: $(OBJS)
.PHONY: all
servidor: servidor.c
	$(CC) -o $@.out $(CFLAGS) $^ 

cliente: cliente.c
	$(CC) -o $@.out $(CFLAGS) $^

libclaves: claves/claves.c
	gcc -fPIC -c -o claves.o $^
	gcc -shared -fPIC -o $@.so claves.o

clean:
	rm -f $(BIN_FILES) *.out *.o *.so
.PHONY: clean


