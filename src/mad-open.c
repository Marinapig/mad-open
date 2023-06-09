#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <fcntl.h>

#include <err.h>

#include <unistd.h>

#include <magic.h>

#include "file.h"
#include "magic.h"
#include "pledge.h"

#define FLAG_TEXT_GENERIC 1 << 0
#define FLAG_FILE_EXTENSION 1 << 1

int main(int argc, char **argv)
{
	if (argc < 2)
		return EXIT_FAILURE;
	int ch;
	char *cval = 0;
	int flags = 0;
	while ((ch = getopt(argc, argv, "tc:ep")) != -1)
	{
		switch (ch)
		{
			case 'c': 
			if (!optarg || *optarg == ':')
			{
				err(1, "");
			}
			cval = optarg;
			break;
			case 't':
			flags |= FLAG_TEXT_GENERIC;
			break;
			case 'e':
			flags |= FLAG_FILE_EXTENSION;
			break;
		}
	}
	char *filename = 0;
	if (!cval)
		filename = getConfigFile();
	else
		filename = cval;

	if (!filename)
	{
		err(1, "Couldn't get rules filename");
	}

	unveil(argv[optind], "r");
	unveil(filename, "r");
	unveil("/usr/local/share/misc/magic.mgc", "r");
	unveil(0, 0);
	if (pledge("rpath stdio", 0) == -1)
		err(1, "pledge");

	char *mime;
	if (flags & FLAG_FILE_EXTENSION)
		mime = getFileExt(argv[optind]);
	else
		mime = get_mimetype(argv[optind], flags & FLAG_TEXT_GENERIC);

	if (!mime)
	{
		if (flags & FLAG_FILE_EXTENSION)
			puts("-e used when file has no extension");
		else
			perror("Couldn't get mimetype");
		free(filename);
		exit(EXIT_FAILURE);
	}

	Association found;
	bool didfind = magic_grep(filename, mime, &found);
	pledge("stdio", 0);
	if (!cval) 
		free(filename);
	if (didfind)
	{
		if (!(flags & FLAG_FILE_EXTENSION))
			free(mime);
		printf("%s -- %s", found.nofork ? "-o" : "", found.program);
	}
	else
	{
		fprintf(stderr, "No program associated with %s\n", mime);
		if (!(flags & FLAG_FILE_EXTENSION))
			free(mime);
	}
}
