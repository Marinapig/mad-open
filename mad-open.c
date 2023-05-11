#include <stdio.h>
#include <stdlib.h>
#include <magic.h>
#include <string.h>
#include <unistd.h>

#define RULES_SIZE 255

typedef struct
{
	char mime[24];
	char program[24];
} Association;

static Association rules[RULES_SIZE];

static void init(void)
{
	FILE *file = fopen("rules", "r");
	if (!file)
		return;
	char *line;
	size_t size;
	for(int i = 0; i < RULES_SIZE; i++)
	{
		if (getline(&line, &size, file) == -1)
		{
			free(line);
			break;
			sscanf(line, "%23s %23s", rules[i].mime, rules[i].program);
		}
		sscanf(line, "%23s %23s", rules[i].mime, rules[i].program);
	}
	fclose(file);
}

int which(const char *s)
{
	for(int i = 0; i < RULES_SIZE; i++)
	{
		if (!strcmp(s, rules[i].mime) )
			return i;
	}
	return -1;
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
	if (!mime)
	{
		magic_close(magic);
		return EXIT_FAILURE;
	}
	init();
	int wh = which(mime);
	magic_close(magic);
	if (wh != -1)
	{
		char *args[] = { rules[wh].program, argv[1] , 0};
		execvp(args[0], args);
	}
}
