CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lcurses

test_client: main.c setup.c test_client main.c  auth.c calendar.c chat.c common.c first_page.c group_utils.c my_page.c setup.c timer_utils.c dummy_data.c
	$(CC) $(CFLAGS) -o test_client main.c auth.c calendar.c chat.c common.c first_page.c group_utils.c my_page.c setup.c timer_utils.c dummy_data.c $(LIBS)

client: main.c setup.c
	$(CC) $(CFLAGS) -o client main.c auth.c calendar.c chat.c common.c first_page.c group_utils.c my_page.c setup.c timer_utils.c $(LIBS)

clean:
	rm -f client
