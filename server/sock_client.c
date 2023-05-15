#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "server.h"

void	client_read(t_server *server, int cs)
{
	int	r;
	int	i;

	r = recv(cs, server->fds[cs].buf_read, BUF_SIZE, 0);
	if (r <= 0)
	{
		close(cs);
		clean_fd(&server->fds[cs]);
		printf("client #%d leave the server\n", cs);
	}
	else
	{
		i = 0;
		while (i < server->maxfd)
		{
			if ((server->fds[i].type == FD_CLIENT) && (i != cs))
				send(i, server->fds[cs].buf_read, r, 0);
			i++;
		}
	}
}

void	client_write(t_server *server, int cs)
{
}
