#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "utils.h"

static void parseuserfile(t_server* server, char *file)
{
	size_t bufsiz = BUFSIZ;
	char *buffer = (char *)malloc(sizeof(char) * bufsiz);

	t_user *newuser = (t_user *)malloc(sizeof(t_user));
	newuser->fd =  -1;
	newuser->id = (char *)malloc(sizeof(char) * MAXIDLEN);
	newuser->passwd= (char *)malloc(sizeof(char) * MAXPSWDLEN);

	char *fullpath = strjoin(USERPATH, file);
	FILE* userfile = (FILE *)Xv(NULL, fopen(fullpath, "r"), "fopen");
	free(fullpath);

	ssize_t linesize = X(-1, getdelim(&buffer, &bufsiz, ' ',  userfile), "getdelim");
	buffer[linesize - 1] = '\0';
	strncpy(newuser->id, buffer, linesize);
	linesize = X(-1, getdelim(&buffer, &bufsiz, '\n', userfile), "getdelim");
	buffer[linesize - 1] = '\0';
	strncpy(newuser->passwd, buffer, linesize);
	newuser->elapsed = getfocustime(file, gettodaydate());
	t_list *newnode = lstnew(newuser);
	lstadd_back(&(server->users), newnode);
	free(buffer);
	fclose(userfile);
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
	size_t	bufsiz = BUFSIZ;
	ssize_t linesize;
	char	*token;
	char	*buffer = (char *)malloc(sizeof(char) * bufsiz);
	FILE*	fp = fopen(GROUPPATH, "r");

	while (1)
	{
		linesize = getdelim(&buffer, &bufsiz, '\n',  fp);
		if (linesize <= 0)
			break;
		buffer[linesize - 1] = '\0';
		t_group *newgroup = (t_group *)malloc(sizeof(t_group));
		token = strtok(buffer, ",");
		newgroup->name = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(newgroup->name, token);
		token = strtok(NULL, ",");
		newgroup->total_elapsed = atoi(token);
		newgroup->op_user = NULL;
		newgroup->joined_users = NULL;

		linesize = X(-1, getdelim(&buffer, &bufsiz, '\n',  fp), "getdelim");
		buffer[linesize - 1] = '\0';
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
						lstadd_back(&(newgroup->op_user), node);
						op_flag = 1;
					}
					else
						lstadd_back(&(newgroup->joined_users), node);
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
