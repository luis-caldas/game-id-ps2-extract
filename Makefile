CC = gcc
CFLAGS = -Wall
PROGRAM_NAME = game-id-ps2-extract

all:
	$(CC) $(CFLAGS) -c memrmem.c -o memrmem.o
	$(CC) $(CFLAGS) memrmem.o main.c -o $(PROGRAM_NAME)

clean:
	rm -f *.o $(PROGRAM_NAME)
