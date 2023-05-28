#include <stdio.h>
#include <stdlib.h>
#include "server.h"

static void	set_port(t_server* server, int ac, char **av)
{
	if (ac != 2)
	{
	  fprintf(stderr, USAGE, av[0]);
	  exit(EXIT_FAILURE);
	}
	server->port = atoi(av[1]);
}

int main(int ac, char *av[])
{
	t_server server;
	
	server.users=NULL;
	server.groups=NULL;

	readfromfile(&server);
	init_env(&server);
	set_port(&server, ac, av);
	srv_create(&server, server.port);
	loop(&server);
	//TODO: server cleanup procedure

	return (0);
}
