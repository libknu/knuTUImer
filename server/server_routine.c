#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "server.h"

void	login(t_server *server, char *input, int cs)
{
	char* id;
	char* passwd;
	
	//parse input
	id = strtok(input, ",");
	passwd = strtok(NULL, ",");
	
	//find user by id and check passwd
	for (t_list *tmp = server->users; tmp != NULL; tmp = tmp->next)
	{
		t_user *user = (t_user *)tmp->content;
		if ((strcmp(user->id, id) == 0) && (strcmp(user->passwd, passwd) == 0))
		{
			send(cs, );
			return ;
		}
	}
	//loose connection
}

void	signin(t_server *server, char *input, int cs)
{
	char* id;
	char* passwd;

	id = strtok(input, ",");
	passwd = strtok(NULL, ",");

	//create new user and push back to the list
	
}

void	privatechat(t_server *server, char *input, int cs)
{
	char* user;
	char *content;

	user = strtok(input, ",");
	content = strtok(NULL, ",");

	//find user by id and send the content
}

void	groupchat(t_server *server, char *input, int cs)
{
	char *groupname;
	char *content;

	groupname = strtok(input, ",");
	content = strtok(NULL, ",");

	//find group by name and send the content
}

void	grouplist(t_server *server, char *input, int cs)
{
	//send user's group list
}

void	groupmember(t_server *server, char *input, int cs)
{
}

void	groupjoin(t_server *server, char *input, int cs)
{
}

void	groupcreate(t_server *server, char *input, int cs)
{
	//create new group and push back to the list

	t_group *new_group = (t_group*)Xv(NULL, malloc(sizeof(*new_group)), "malloc");
	new_group->name = input;

	lstadd_back(&server->groups, lstnew(new_group));
	//send SUCCESS to client	
}
