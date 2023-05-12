#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "magic_private.h"

void cache_build(void)
{
	char *cachename = getConfigFile(true);
	FILE *file = fopen(cachename, "w");
	free(cachename);
	if (!file)
	{
		perror("Couldn't open cache file");
		exit(EXIT_FAILURE);
	}
	size_t size = sizeof(rules);
	if (fwrite(&rules, size, 1, file) != 1)
	{
			perror("Cannot write to cache file");
			fclose(file);
			exit(EXIT_FAILURE);
	}
	fclose(file);
}

void cache_read(void)
{
	char *cachename = getConfigFile(true);
	FILE *file = fopen(cachename, "r");
	free(cachename);

	if (!file)
	{
		perror("Couldn't open cache file for reading");
		exit(EXIT_FAILURE);
	}
	size_t size = sizeof(rules);
	if (fread(&rules, size, 1, file) != 1)
	{
		perror("Cannot read from cache file");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	fclose(file);
}

int shouldCache(void)
{
	char *filename = getConfigFile(false);
	char *cachename = getConfigFile(true);
	if (file_creationTime(cachename) == 0 || (file_creationTime(filename) > file_creationTime(cachename) ) )
	{
		free(filename);
		free(cachename);
		return false;
	}
	free(filename);
	free(cachename);
	return true;
}

bool cacheExists(void)
{
	return (file_creationTime("/home/user/madcache") != 0);
}
