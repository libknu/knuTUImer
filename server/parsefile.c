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
}

void	readfromfile(t_server *server)
{
	getusersinfo(server);
	getgroupsinfo(server);
}
