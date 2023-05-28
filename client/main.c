#include "client.h"

int main(int argc, char **argv){
	int sock_fd;
	sock_fd = setup(argc, argv);
    get_terminal_size();

    first_page(sock_fd);
}