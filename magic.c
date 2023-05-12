#include "magic.h"
#include <string.h>
#include <magic.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#include "file.h"
#include "char.h"

#include "magic_private.h"

//#define RULES_SIZE 255

Association rules[RULES_SIZE];

void magic_init(char *s)
{
	FILE *file;
	if (!s)
	{
		char *filename = getConfigFile(false);
		file = fopen(filename, "r");
		free(filename);
	}
	else
		file = fopen(s, "r");

	if (!file)
	{
		perror("Couldn't open config file");
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
		int tempfork = false;
		sscanf(san, "%64s %24s %d", rules[i].mime, rules[i].program, &tempfork);
		rules[i].nofork = tempfork;
		free(san);
	}
	fclose(file);
}

static Association* magic_which(const char *s)
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

Association* magic_getassociation(const char *s, bool text_generic)
{
	magic_t magic;
	const char *mime;

	magic = magic_open(MAGIC_MIME_TYPE);
	if (!magic)
		return 0;
	if (text_generic) //Treat all text files as the same mime type
		magic_setflags(magic, magic_getflags(magic) | MAGIC_NO_CHECK_TEXT);

	magic_load(magic, NULL);
	mime = magic_file(magic, s);
	if (!mime || strstr(mime, "No such") )
	{
		magic_close(magic);
		perror("Couldn't get mime type from file");
		return 0;
	}
	Association *found = magic_which(mime);
	if (!found)
		fprintf(stderr, "No rule association for %s\n", mime);
	magic_close(magic);
	return found;
}
