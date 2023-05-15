#include <sys/select.h>
#include <string.h>
#include "server.h"

void	init_fd(t_server *server)
{
	int	i;

	i = 0;
	server->max = 0;
	FD_ZERO(&server->fd_read);
	FD_ZERO(&server->fd_write);
	while (i < server->maxfd)
	{
		if (server->fds[i].type != FD_FREE)
		{
			FD_SET(i, &server->fd_read);
			if (strlen(server->fds[i].buf_write) > 0)
				FD_SET(i, &server->fd_write);
			server->max = MAX(server->max, i);
		}
		i++;
	}
}

void	loop(t_server* server)
{
	while (1)
	{
		init_fd(server);	
		server->r = select(server->max + 1, &server->fd_read, &server->fd_write, NULL, NULL);
		for (int i = 0; (i < server->maxfd) && (server->r > 0); ++i)
		{
			if (FD_ISSET(i, &server->fd_read))
				server->fds[i].fct_read(server, i);
			else if (FD_ISSET(i, &server->fd_write))
				server->fds[i].fct_write(server, i);
			else
				continue;
			server->r--;
		}
	}
}
