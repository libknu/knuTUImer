#include "server.h"

void	set_port(t_server& server, int ac, char **av)
{
	if (ac != 2)
	{
	  fprintf(stderr, USAGE, av[0]);
	  exit(EXIT_FAILURE);
	}
	server->port = atoi(av[1]);
}


int main(int ac, char *av)
{
	t_server server;

	init_env(&server);
	set_port(&server, ac, av);
	srv_create(&server, server.port);
	loop(&server);

	return (0);
}
