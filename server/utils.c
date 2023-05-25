#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "server.h"
#include "utils.h"

char *gettodaydate()
{
	time_t t;
	struct tm *timeinfo;
	char	*date_str = (char*)malloc(sizeof(char) * 11);

	time(&t);
	timeinfo = localtime(&t);
	strftime(date_str, 11, "%Y-%m-%d", timeinfo);
	return (date_str);
}

long long getfocustime(char *user, char *date)
{
	long long elapsed;
	char *buffer = (char *)malloc(sizeof(char) * BUFSIZ);
	char *command;
	char *userfilepath = strjoin(USERPATH, user);

	char *cmd1 = "cat ";
	char *cmd2 = " | grep ";
	char *cmd3 = " | awk '{print $2}'";
	size_t size = strlen(cmd1) + strlen(cmd2) + strlen(cmd3) + strlen(userfilepath) + strlen(date) + 2;

	command = (char *)malloc(sizeof(char) * size);
	strcpy(command, cmd1);
	strcat(command, userfilepath);
	strcat(command, cmd2);
	strcat(command, date);
	strcat(command, cmd3);
	free(date); //NOTE: check if the date is dynamically allocated

	FILE* fp = popen(command, "r");
	fgets(buffer, BUFSIZ, fp);
	int status = pclose(fp);
	elapsed = WIFEXITED(status) ? 0 : atoi(buffer);
	free(command);
	free(userfilepath);
	free(buffer);
	return elapsed;
}

char* strjoin(const char* str1, const char* str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1); 

    if (result != NULL)
	{
        strcpy(result, str1);
        strcat(result, str2);
    }
    return result;
}
