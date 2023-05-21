#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <stdio.h>
#include "client.h"

int	setup(int ac, char **av)//TODO: exception handling
{
	int					sock_fd;
	struct sockaddr_in	servadd;
	struct hostent		*hp;

	sock_fd = socket(PF_INET, SOCK_STREAM, 0);

	bzero(&servadd, sizeof(servadd));
	hp = gethostbyname(av[1]);
	bcopy(hp->h_addr, (struct sockeaddr*)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(atoi(av[2]));
	servadd.sin_family = AF_INET;

	connect(sock_fd, (struct sockaddr*)&servadd, sizeof(servadd));
	return (sock_fd);
}
