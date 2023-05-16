#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

char *removechar(char needle, const char *haystack, bool *found)
{
	assert(found != NULL && "Bool passed to removechar was not initialized");
	/*
	if (!found)
	{
		perror("Bool passed to removechar was not initialized");
		return NULL;
	}
	*/
	char *ret = calloc(strlen(haystack) + 1, sizeof(char));
	if (!ret)
	{
		perror("Couldn't allocate memory");
		return NULL;
	}
	char *start = ret;
	while (*haystack)
	{
		if (*haystack == needle)
		{
			*found = true;
		}
		else
		{
			*start = *haystack;
			++start;
		}
		++haystack;
	}
	start = 0;
	return ret;
}


