#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <regex.h>
#include <unistd.h>

#include <magic.h>

#include "file.h"
#include "magic.h"

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
		return 0;
	}
	char *ret = calloc(strlen(mime) + 1, sizeof(char));
	strcpy(ret, mime);
	magic_close(magic);
	return ret;
}

static inline void reset_rule(Association *rule)
{
	rule->mime[0] = '\0';
	rule->nofork = false;
}

bool magic_grep(const char *filename, const char *mime, Association *rule)
{
	assert(filename);
	assert(mime);
	assert(rule);

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
	char options[255];
	while (getline(&line, &size, file) != -1)
	{
		options[0] = '\0';
		reset_rule(rule);
		sscanf(line, "%64s %24s %255s", rule->mime, rule->program, options);
		rule->nofork = strstr(options, "noclose");
		regex_t regex;
		if (regcomp(&regex, rule->mime, REG_NEWLINE))
			exit(EXIT_FAILURE);
		if (!regexec(&regex, mime, 0, NULL, 0))
			ret = true;
		regfree(&regex);
		if (ret)
			break;
	}
	fclose(file);
	free(line);
	return ret;
}
