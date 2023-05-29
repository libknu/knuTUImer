#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "server.h"

void	client_read(t_server *server, int cs)
{
	int	r;

	r = recv(cs, server->fds[cs].buf_read, BUF_SIZE, 0);
	if (r <= 0)
	{
		close(cs);
		clean_fd(&server->fds[cs]);
		printf("client #%d leave the server\n", cs);
	}
	else
	{
		printf("%d bytes from %d, %s\n", r, cs, server->fds[cs].buf_read);
		if (strncmp(server->fds[cs].buf_read, "login:", 6) == 0)
			login(server, server->fds[cs].buf_read + 6, cs);
		else if (strncmp(server->fds[cs].buf_read, "signin:", 7) == 0)
			signin(server, server->fds[cs].buf_read + 7, cs);
		else if (strncmp(server->fds[cs].buf_read, "chat:", 5) == 0)
			groupchat(server, server->fds[cs].buf_read + 6, cs);
		else if (strncmp(server->fds[cs].buf_read, "grouplist:", 10) == 0)
			grouplist(server, server->fds[cs].buf_read + 10, cs);
		else if (strncmp(server->fds[cs].buf_read, "member:", 7) == 0)
			groupmember(server, server->fds[cs].buf_read + 7, cs);
		else if (strncmp(server->fds[cs].buf_read, "join:", 5) == 0)
			groupjoin(server, server->fds[cs].buf_read + 5, cs);
		else if (strncmp(server->fds[cs].buf_read, "create:", 7) == 0)
			groupcreate(server, server->fds[cs].buf_read + 7, cs);
		else if (strncmp(server->fds[cs].buf_read, "time:", 5) == 0)
			focustime(server, server->fds[cs].buf_read + 5, cs);
		else if (strncmp(server->fds[cs].buf_read, "attendance:", 11) == 0)
			getattendance(server, server->fds[cs].buf_read + 11, cs);
		else if (strncmp(server->fds[cs].buf_read, "focus:", 6) == 0)
			focustime(server, server->fds[cs].buf_read + 6, cs);
		else
			printf("undefined input: %s\n", server->fds[cs].buf_read);
	}
	memset(server->fds[cs].buf_read, 0, BUFSIZ);
}

void	client_write(t_server *server, int cs)
{
}
