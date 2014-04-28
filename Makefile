CC=gcc
CFLAGS=-Wall -Wextra
DEPS = define.h parameter.h structs.h
OBJ = main.o common.o read_circuit.o hashes.o structures.o read_vectors.o lut.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm main $(OBJ)
