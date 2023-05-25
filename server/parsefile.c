#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "server.h"
#include "utils.h"

# define GROUPPATH "../db"
# define USERPATH "../db/users"

# define MAXIDLEN 255
# define MAXPSWDLEN 10

static void parseuserfile(t_server* server, char *file)
{
	char *buffer = (char *)malloc(sizeof(char) * BUFSIZ);

	t_user *newuser = (t_user *)malloc(sizeof(t_user));
	newuser->id = (char *)malloc(sizeof(char) * MAXIDLEN);
	newuser->passwd= (char *)malloc(sizeof(char) * MAXPSWDLEN);

	t_list *newnode = (t_list *)malloc(sizeof(t_list));
	FILE* userfile = fopen(file, "r");

	ssize_t linesize = getdelim(&buffer, 0, ' ',  userfile);
	strncpy(newuser->id, buffer, linesize);
	linesize = getdelim(&buffer, 0, ' ',  userfile);
	strncpy(newuser->passwd, buffer, linesize);
	newuser->elapsed  = getfocustime(file, gettodaydate());
	newnode->content = newuser;
	newnode->next = NULL;
	lstadd_back(&(server->users), newnode);
	free(buffer);
}

static void getusersinfo(t_server *server)
{
	struct dirent* entry;
	//get file entry from user directory
	DIR *directory = (DIR *)Xv(NULL, opendir(USERPATH), "opendir");
    while ((entry = readdir(directory)) != NULL)
	{
        if (strcmp(entry->d_name, ".") == 0 \
				|| strcmp(entry->d_name, "..") == 0)
            continue;

		parseuserfile(server, entry->d_name);
    }
	closedir(directory);
}

static void getgroupsinfo(t_server *server)
{
	int		op_flag;
	ssize_t	linesize;
	char	*token;
	char	*buffer = (char *)malloc(sizeof(char) * BUFSIZ);
	FILE*	fp = fopen(GROUPPATH, "r");

	while (1)
	{
		linesize = getdelim(&buffer, 0, '\n',  fp);
		if (linesize == -1)
			break;
		t_group *newgroup = (t_group *)malloc(sizeof(t_group));
		token = strtok(buffer, ",");
		newgroup->name = (char *)malloc(sizeof(char) * strlen(token));
		strcpy(newgroup->name, token);
		token = strtok(NULL, ",");
		newgroup->total_elapsed = atoi(token);

		linesize = getdelim(&buffer, 0, '\n',  fp);
		token = strtok(buffer, ",");
		op_flag = 0;
		while (token != NULL)
		{
			t_list *node = server->users;
			while (node != NULL)
			{
				if (strcmp(((t_user *)node->content)->id, token) == 0)
				{
					if (op_flag == 0)
					{
						lstadd_back(&(newgroup->op_user), lstnew(node->content));
						op_flag = 1;
					}
					else
						lstadd_back(&(newgroup->joined_users), lstnew(node->content));
					break;
				}
				node = node->next;
			}
			token = strtok(NULL, ",");
		}
		lstadd_back(&(server->groups), lstnew(newgroup));
	}
	fclose(fp);
}

void	readfromfile(t_server *server)
{
	getusersinfo(server);
	getgroupsinfo(server);
}
