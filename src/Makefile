CC=gcc
CFLAGS=-Wall -Wextra -c
EXE=executabil
run:
	./$(EXE)
build: tema3.o
	$(CC) $^ -o $(EXE)

tema3.o: tema3.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf tema3.o $(EXE)

