#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <sys/select.h>

# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))

# define USAGE		"Usage: %s port\n"

# define BACKLOG	128 //sysctl kern.ipc.somaxconn for macos
						//sysctl net.core.somaxconn for ubutu

typedef struct s_server
{
	//group list
	//users logged in
	//server info
	t_fd	*fds;
	int		port;
	int		maxfd;
	int		max;
	int		r;
	fd_set	fd_read;
	fd_set	fd_write;
}t_server;

//init_env.c
void	init_evn(t_server*);
void	clean_fd(t_fd*);

//srv_create.c
void	srv_create(t_server*, int);

#endif
