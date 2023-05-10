#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <sys/select.h>

# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))

typedef struct s_server
{
	//group list
	//users logged in
	//server info
}t_server;

#endif
