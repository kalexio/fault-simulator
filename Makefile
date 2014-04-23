CC=gcc
CFLAGS=-Wall
DEPS = define.h parameter.h structs.h
OBJ = main.o common.o read_circuit.o hashes.o structures.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm main $(OBJ)
