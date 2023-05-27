#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "server.h"
#include "utils.h"

void	login(t_server *server, char *input, int cs)
{
	char* id;
	char* passwd;
	
	id = strtok(input, ",");
	passwd = strtok(NULL, ",");
	for (t_list *tmp = server->users; tmp != NULL; tmp = tmp->next)
	{
		t_user *user = (t_user *)tmp->content;
		if ((strcmp(user->id, id) == 0) && (strcmp(user->passwd, passwd) == 0))
		{
			server->fds[cs].user = user; send(cs, "SUCCESS", 7, 0);
			return ;
		}
	}
	close(cs); //TODO: collect resources
}

void	signin(t_server *server, char *input, int cs)
{
	char*	id;
	char*	passwd;
	t_user	*newuser;

	id = strtok(input, ",");
	passwd = strtok(NULL, ",");
	newuser = (t_user*)malloc(sizeof(t_user));
	newuser->fd = cs;
	newuser->id = id;
	newuser->passwd = passwd;
	newuser->elapsed = 0;
	lstadd_back(&server->users, lstnew(newuser));
	send(cs, "SUCCESS", 7, 0);
}

void	groupchat(t_server *server, char *input, int cs)
{
	char *groupname;
	char *content;
	char *msg;

	groupname = strtok(input, ",");
	content = strtok(NULL, ",");
	msg = strjoin("chat:", content);

	//find group by name and send the content
	for (t_list* tmp = server->groups; tmp != NULL; tmp = tmp->next)
	{
		t_group *group = (t_group*)tmp->content;
		if (strcmp(group->name, groupname) == 0)
		{
			for (t_list* usr = group->joined_users; usr != NULL; usr = usr->next)
				send(((t_user *)usr->content)->fd, msg, strlen(msg), 0);
			send(((t_user *)group->op_user->content)->fd, msg, strlen(msg), 0);
			break;
		}
	}
	free(msg);
}

void	grouplist(t_server *server, char *input, int cs)
{
	//send user's group list
	char *msg = (char *)malloc(sizeof(char) * BUFSIZ);
	strcpy(msg, "grouplist:");
	for (t_list *group_tmp = server->groups; group_tmp != NULL; group_tmp = group_tmp->next)
	{
		if (userinthegroup(group_tmp->content, cs))
		{
			strlcat(msg, ((t_group *)group_tmp->content)->name, BUFSIZ);
			strlcat(msg, ",", BUFSIZ);
		}
	}
	msg[strlen(msg) - 1] = '\0';
	send(cs, msg, strlen(msg), 0);
	free(msg);
}

void	groupmember(t_server *server, char *input, int cs)
{
	t_group *group;
	char *msg = (char *)malloc(sizeof(char) * BUFSIZ);

	strcpy(msg, "member:");
	for (t_list *tmp = server->groups; tmp != NULL; tmp = tmp->next)
	{
		if (strcmp(((t_group *)tmp->content)->name, input) == 0)
			group = (t_group *)tmp;
	}
	strlcat(msg, ((t_user *)group->op_user->content)->id, BUFSIZ);
	strlcat(msg, ",", BUFSIZ);
	for (t_list *tmp = group->joined_users; tmp != NULL; tmp = tmp->next)
	{
		strlcat(msg, ((t_user *)tmp->content)->id, BUFSIZ);
		strlcat(msg, ",", BUFSIZ);
	}
	msg[strlen(msg) - 1] = '\0';
	send(cs, msg, strlen(msg), 0);
	free(msg);
}

void	groupjoin(t_server *server, char *input, int cs)
{
	t_group *group;
	t_list	*user;
	for (t_list *tmp = server->groups; tmp != NULL; tmp = tmp->next)
	{
		if (strcmp(((t_group*)tmp->content)->name, input) == 0)
			group = (t_group*)tmp;
	}
	for (t_list *tmp = server->users; tmp != NULL; tmp = tmp->next)
	{
		if (((t_user *)tmp->content)->fd == cs)
			user = tmp;
	}
	lstadd_back(&group->joined_users, user);
	send(cs, "SUCCESS", 7, 0);
}

void	groupcreate(t_server *server, char *input, int cs)
{
	t_list	*user;
	t_group *new_group = (t_group*)Xv(NULL, malloc(sizeof(*new_group)), "malloc");
	new_group->name = input;
	for (t_list *tmp = server->users; tmp != NULL; tmp = tmp->next)
	{
		if (((t_user *)tmp->content)->fd == cs)
			user = tmp;
	}
	new_group->op_user = user;
	new_group-> joined_users = NULL;
	lstadd_back(&server->groups, lstnew(new_group));
	send(cs, "SUCCESS", 7, 0);
}

void	focustime(t_server *server, char *input, int cs)
{
	char	*msg = (char *)malloc(sizeof(char) * BUFSIZ);
	strcpy(msg, "focus:");
	strlcat(msg, itoa(server->fds[cs].user->elapsed), BUFSIZ);
	send(cs, msg, strlen(msg), 0);
	free(msg);
}

void	getattendance(t_server *server, char *input, int cs)
{
	int month = atoi(input);
	(void)month;
	char *msg = (char *)malloc(sizeof(char) * BUFSIZ);
	char *user;
	for (t_list *tmp = server->users; tmp != NULL; tmp = tmp->next)
	{
		if (((t_user *)tmp->content)->fd == cs)
			user = ((t_user *)tmp->content)->id;
	}
	strcpy(msg, "attendance:");
	char date[11] = "2023-05-00";
	for (int i = 1; i <= 9; ++i)
	{
		date[9] = i + '0';
		if (getfocustime(date, user) == 0)
		{
			strlcat(msg, "0", BUFSIZ);
			strlcat(msg, ",", BUFSIZ);
		}
		else
		{
			strlcat(msg, "1", BUFSIZ);
			strlcat(msg, ",", BUFSIZ);
		}
	}
	date[8] = '1';
	for (int i = 1; i <= 9; ++i)
	{
		date[9] = i + '0';
		if (getfocustime(date, user) == 0)
		{
			strlcat(msg, "0", BUFSIZ);
			strlcat(msg, ",", BUFSIZ);
		}
		else
		{
			strlcat(msg, "1", BUFSIZ);
			strlcat(msg, ",", BUFSIZ);
		}
	}
	date[8] = '2';
	for (int i = 1; i <= 9; ++i)
	{
		if (getfocustime(date, user) == 0)
		{
			strlcat(msg, "0", BUFSIZ);
			strlcat(msg, ",", BUFSIZ);
		}
		else
		{
			strlcat(msg, "1", BUFSIZ);
			strlcat(msg, ",", BUFSIZ);
		}
	}
	date[8] = '3';
	date[9] = '0';
	if (getfocustime(date, user) == 0)
		strlcat(msg, "0", BUFSIZ);
	else
		strlcat(msg, "1", BUFSIZ);
	send(cs, msg, strlen(msg), 0);
	free(msg);
}
