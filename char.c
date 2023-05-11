#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char *removechar(char needle, const char *haystack, bool *found)
{
	char *ret = calloc(strlen(haystack) + 1, sizeof(char));

	if (!ret)
	{
		perror("Couldn't allocate memory");
		return 0;
	}
	char *start = ret;
	while (*haystack)
	{
		if (*haystack == needle)
		{
			*found = true;
			goto incstack;
		}
		*start = *haystack;
		++start;
		incstack:
		++haystack;
	}
	start = '\0';
	return ret;
}


