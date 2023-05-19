CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lcurses

client: main.c setup.c
	$(CC) $(CFLAGS) -o client main.c setup.c $(LIBS)

clean:
	rm -f client
