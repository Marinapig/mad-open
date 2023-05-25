#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include <regex.h>
#include <unistd.h>
#include <err.h>

#include <magic.h>

#include "file.h"
#include "magic.h"


char *get_mimetype(const char *filename, bool text_generic)
{
	assert(filename);

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
	size_t size = strlen(mime) + 1;
	char *ret = calloc(size, sizeof(char));
	memcpy(ret, mime, size);
	magic_close(magic);
	return ret;
}

static inline void reset_rule(Association *rule)
{
	assert(rule);

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
		err(1, "Couldn't open rules file");
	}

	bool ret = false;
	char *line = 0;
	size_t size;
	while (getline(&line, &size, file) != -1)
	{
		reset_rule(rule);
		char* mime_p = rule->mime;
		char *line_p = line;
		while (line_p && !isspace(*line_p))
		{
			*(mime_p++) = *(line_p++);
		}
		*(mime_p) = '\0';
		++line_p;
		regex_t regex;
		if (regcomp(&regex, rule->mime, 0))
			exit(EXIT_FAILURE);
		if (!regexec(&regex, mime, 0, NULL, 0))
		{
			char *prog_p = rule->program;
			while (line_p && prog_p && *line_p != '\n' && (strncmp(line_p, " options", strlen(" options") ) != 0))
			{
				*(prog_p++) = *(line_p++);
			}
			*(prog_p) = '\0';
			rule->nofork = strstr(line_p, "noclose");
			ret = true;
		}
		regfree(&regex);
		if (ret)
			break;
	}
	fclose(file);
	free(line);
	return ret;
}
