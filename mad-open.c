#include <stdio.h>
#include <stdlib.h>
#include <magic.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <fcntl.h>

#include "file.h"
#include "char.h"

#define RULES_SIZE 255

typedef struct
{
	char mime[24];
	char program[24];
	bool wildcard;
} Association;

static Association rules[RULES_SIZE];

static void init(void)
{
	char *filename = getConfigFile();
	FILE *file = fopen(filename, "r");
	free(filename);
	if (!file)
	{
		perror("Couldn't open file");
		exit(EXIT_FAILURE);
	}
	char *line = 0;
	size_t size;
	for(int i = 0; i < RULES_SIZE; i++)
	{
		if (getline(&line, &size, file) == -1)
		{
			free(line);
			break;
		}
		rules[i].wildcard = false;
		char *san = removechar('*', line, &rules[i].wildcard); 
		if (!san)
		{
			perror("removechar failed");
			exit(EXIT_FAILURE);
		}
		sscanf(san, "%23s %23s", rules[i].mime, rules[i].program);
		free(san);
	}
	fclose(file);
}

Association* which(const char *s)
{
	for(int i = 0; i < RULES_SIZE; i++)
	{
		if (rules[i].wildcard)
		{
			if (strstr(s, rules[i].mime) )
				return &rules[i];
		}
		else if (!strcmp(s, rules[i].mime) )
			return &rules[i];
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return EXIT_FAILURE;
	magic_t magic;
	const char *mime;

	magic = magic_open(MAGIC_MIME_TYPE);
	if (!magic)
		return EXIT_FAILURE;
	magic_load(magic, NULL);
	mime = magic_file(magic, argv[1]);
	if (!mime || strstr(mime, "No such file or directory") ) //magic_file still returns a string even if it cant find the file, there might be more errors of this nature
	{
		magic_close(magic);
		perror("Couldn't open the file");
		return EXIT_FAILURE;
	}
	init();
	Association *found = which(mime);
	bool shouldfork = ( strstr(mime, "text/") ) == NULL; //Maybe this should be specifiable in the rules file
	if (found)
	{
		magic_close(magic);
		char *args[] = { found->program, argv[1] , 0};
		#ifndef NDEBUG
		if (shouldfork)
		{
			int fd = open("/dev/null", O_WRONLY);
			if (fd == -1)
			{
				perror("Could not open file");
				return EXIT_FAILURE;
			}
			if (dup2(fd, 1) == -1 || dup2(fd, 2) == -1)
			{
				perror("One or more calls to dup2 failed");
				close(fd);
				return EXIT_FAILURE;
			}
			close(fd);
		}
		execvp(args[0], args);
		#else
		printf("%s %s\n", args[0], args[1]);
		#endif
	}
	else
	{
		printf("Couldn't match %s to a rule\n", mime);
		magic_close(magic);
	}
}
