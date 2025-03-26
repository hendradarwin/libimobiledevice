#include "libgen.h"

#define MAX_PATH 260
#define NULL 0

char *basename(char *path)
{
	static char name[MAX_PATH];
	int i;
	if (path == NULL || strlen(path) == 0)
		return (char*)".";
	i = strlen(path) - 1;
	while (path[i] != '\\' && path[i] != '/' && i >= 0)
		i--;
	strncpy(name, path + i + 1, MAX_PATH);
	return name;
}