#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "server.h"

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

	FILE* fp = popen(command, "r");
	fgets(buffer, BUFSIZ, fp);
	int status = pclose(fp);
	elapsed = WIFEXITED(status) ? 0 : atoi(buffer);
	free(command);
	free(buffer);
	free(userfilepath);
	free(date); //NOTE: check if the date is dynamically allocated
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

t_user* unwrap_user(t_list *wr)
{
	return ((t_user*)((t_list*)wr->content)->content);
}

int userinthegroup(t_group *group, int fd)
{
	t_user* op_user = unwrap_user(group->op_user->content);
	if (op_user->fd == fd)
		return 1;
	t_list *userlist = group->joined_users;
	for (t_list *tmp = userlist; tmp != NULL; tmp = tmp->next)
	{
		if (unwrap_user((t_list*)tmp->content)->fd == fd)
			return 1;
	}
	return 0;
}

static int	get_digit_byte(long long n)
{
	int	digit;

	digit = 1;
	while (n / 10)
	{
		n /= 10;
		digit++;
	}
	return (digit);
}

static void	get_digits(char *result, long long tmp, int len)
{
	if (len == 0)
		return ;
	result[--len] = tmp % 10 + '0';
	if (tmp == 0)
		return ;
	get_digits(result, tmp / 10, len);
}

char	*itoa(int n)
{
	int			len;
	char		*result;
	long long	tmp;

	tmp = n;
	len = get_digit_byte(tmp);
	if (n < 0)
	{
		len++;
		tmp *= -1;
	}
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	get_digits(result, tmp, len);
	if (n < 0)
		result[0] = '-';
	result[len] = '\0';
	return (result);
}
