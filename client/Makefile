CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lcurses

client: main.c setup.c auth.c calendar.c chat.c common.c first_page.c group_utils.c my_page.c timer_utils.c
	$(CC) $(CFLAGS) -o client main.c  auth.c calendar.c chat.c common.c first_page.c group_utils.c my_page.c setup.c timer_utils.c $(LIBS)

clean:
	rm -f client
