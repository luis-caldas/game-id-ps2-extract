CC = gcc
CFLAGS = -Wall
PROGRAM_NAME = game-id-ps2-extract

all:
	$(CC) $(CFLAGS) -c memmem.c -o memmem.o
	$(CC) $(CFLAGS) -c print.c -o print.o
	$(CC) $(CFLAGS) memmem.o print.o main.c -o $(PROGRAM_NAME)

clean:
	rm -f *.o $(PROGRAM_NAME)
