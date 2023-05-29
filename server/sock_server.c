#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "server.h"

void	srv_create(t_server *server, int port)
{
	int					s;
	int					value;
	struct sockaddr_in	sin;
	struct protoent		*pe;
	

	pe = (struct protoent*)Xv(NULL, getprotobyname("tcp"), "getprotobyname");
	s = X(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto), "socket");
	value = true;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	X(-1, bind(s, (struct sockaddr*)&sin, sizeof(sin)), "bind");
	X(-1, listen(s, BACKLOG), "listen");
	server->fds[s].type = FD_SERV;
	server->fds[s].fct_read = srv_accept;
	server->fds[s].fct_write = NULL;
	server->fds[s].user = NULL;
}

void	srv_accept(t_server *server, int s)
{
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			csin_len;
	
	csin_len = sizeof(csin);
	cs = X(-1, accept(s, (struct sockaddr*)&csin, &csin_len), "accept");
	printf("New client #%d from %s:%d\n", cs,
	   inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
	clean_fd(&server->fds[cs]);
	server->fds[cs].type = FD_CLIENT;
	server->fds[cs].fct_read = client_read;
	server->fds[cs].fct_write = client_write;
}
