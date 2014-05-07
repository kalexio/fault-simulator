CC=gcc
CFLAGS=-Wall -Wextra -g
DEPS = define.h parameter.h structs.h
OBJ = main.o common.o read_circuit.o hashes.o structures.o read_vectors.o logic_sim.o fault_sim.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm main $(OBJ)
