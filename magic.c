#include "magic.h"
#include <string.h>
#include <magic.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#include "file.h"
#include "char.h"

char *get_mimetype(const char *filename, bool text_generic)
{
	magic_t magic;
	const char *mime;

	magic = magic_open(MAGIC_MIME_TYPE);
	if (!magic)
		return 0;
	if (text_generic) //Treat all text files as the same mime type
		magic_setflags(magic, magic_getflags(magic) | MAGIC_NO_CHECK_TEXT);

	magic_load(magic, NULL);
	mime = magic_file(magic, filename);
	if (!mime || strstr(mime, "No such") )
	{
		magic_close(magic);
		perror("Couldn't get mime type from file");
		return 0;
	}
	size_t size = strlen(mime);
	char *ret = calloc(size + 1, sizeof(char));
	strcpy(ret, mime);
	magic_close(magic);
	return ret;
}

bool magic_grep(const char *filename, char *mime, Association *rule)
{
	FILE *file;
	file = fopen(filename, "r");
	if (!file)
	{
		perror("Couldn't open rules file");
		return false;
	}

	bool ret = false;
	char *line = 0;
	size_t size;
	if (!rule)
	{
		perror("Couldn't allocate memory for a rule");
		exit(EXIT_FAILURE);
	}
	while (getline(&line, &size, file) != -1)
	{
		rule->wildcard = false;
		char *san = removechar('*', line, &rule->wildcard);
		if (!san)
		{
			perror("removechar failed");
			exit(EXIT_FAILURE);
		}
		int tempfork = false;
		sscanf(san, "%64s %24s %d", rule->mime, rule->program, &tempfork);
		rule->nofork = tempfork;
		free(san);
		if (rule->wildcard && strstr(mime, rule->mime) || (!strcmp(rule->mime, mime)) )
		{
			ret = true;
			break;
		}
	}
	fclose(file);
	free(line);
	return ret;
}
