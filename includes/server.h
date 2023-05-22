#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <sys/select.h>

# include "list.h"

# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))

# define MAX(a,b)	((a > b) ? a : b)

# define BUF_SIZE	4096

# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

# define USAGE		"Usage: %s port\n"

# define BACKLOG	128 //sysctl kern.ipc.somaxconn for macos
						//sysctl net.core.somaxconn for ubutu

typedef struct s_server t_server;

typedef struct	s_fd
{
  int	type;
  void	(*fct_read)(t_server *, int);
  void	(*fct_write)(t_server *, int);
  char	buf_read[BUF_SIZE + 1];
  char	buf_write[BUF_SIZE + 1];
}t_fd;

typedef struct s_user
{
	char		*id;
	char		*passwd;
	long long	elapsed;
}t_user;

typedef struct s_group
{
	char		*name;
	t_list		*op_user;
	t_list		*joined_users;
	long long	total_elaped;
}t_group;

typedef struct s_server
{
	//group list
	t_list* groups;
	
	//users logged in
	t_list* active_users;

	//users signed in
	t_list* users;

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
void		init_env(t_server*);
void		clean_fd(t_fd*);

//sock_server.c
void		srv_create(t_server*, int);
void		srv_accept(t_server*, int s);

//sock_client.c
void		client_read(t_server*, int cs);
void		client_write(t_server*, int cs);

//x.c
int			x_int(int, int, char*, char*, int);
void		*x_void(void*, void*, char*, char*, int);

//loop.c
void		loop(t_server*);

//parsefile.c
void		readfromfile(t_server*);
long long	getfocustime(char *user, char *date);
char	*gettodaydate();

#endif
