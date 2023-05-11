#include <sys/resource.h>
#include <stdlib.h>
#include "server.h"
	
void	clean_fd(t_fd *fd)
{
  fd->type = FD_FREE;
  fd->fct_read = NULL;
  fd->fct_write = NULL;
}

void	init_evn(t_server *server)
{
	int				i;
	struct rlimit	rlim;

	//get max fd and set
	X(-1, getrlimit(RLIMIT_NOFILE, &rlim), "getrlimit"); //NOFILE - number of open files
	server->maxfd = rlim.rlim_cur;
	server->fds = (t_fd*)Xv(NULL, malloc(sizeof(*server->fds) * server->maxfd), "malloc");
	i = 0;
	while (i < server->maxfd)
	{
	  clean_fd(&server->fds[i]);
	  i++;
	}
}
