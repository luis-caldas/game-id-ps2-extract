CC = gcc
CFLAGS = -Wall
PROGRAM_NAME = game-id-ps2-extract

all:
	$(CC) $(CFLAGS) -c memrmem.c -o memrmem.o
	$(CC) $(CFLAGS) -c print_hex.c -o print_hex.o
	$(CC) $(CFLAGS) memrmem.o print_hex.o main.c -o $(PROGRAM_NAME)

clean:
	rm -f *.o $(PROGRAM_NAME)
