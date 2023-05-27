#ifndef __UTILS_H__
#define __UTILS_H__

#include "server.h"

char		*gettodaydate();
long long	getfocustime(char *user, char *date);
char*		strjoin(const char* str1, const char* str2);
int			userinthegroup(t_group *, int);
char*		itoa(int);

#endif
