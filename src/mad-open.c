#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <fcntl.h>

#include <unistd.h>

#include <magic.h>

#include "file.h"
#include "magic.h"

#define FLAG_TEXT_GENERIC 1 << 0
#define FLAG_FILE_EXTENSION 1 << 1
#define FLAG_PRETEND 1 << 2

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
				perror("");
				return EXIT_FAILURE;
			}
			cval = optarg;
			break;
			case 't':
			flags |= FLAG_TEXT_GENERIC;
			break;
			case 'e':
			flags |= FLAG_FILE_EXTENSION;
			break;
			case 'p':
			flags |= FLAG_PRETEND;
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
		perror("Couldn't get rules filename");
		exit(EXIT_FAILURE);
	}

	#ifdef __OpenBSD__
	//Maybe unveil too, but I need all the stuff in PATH, so I need to figure that out
	pledge("rpath stdio", 0);
	#endif

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
	if (!cval) 
		free(filename);
	#ifdef __OpenBSD__
	pledge("stdio", 0);
	#endif
	if (didfind)
	{
		if (!(flags & FLAG_FILE_EXTENSION))
			free(mime);
		printf("%s", found.program);
	}
	else
	{
		fprintf(stderr, "No program associated with %s\n", mime);
		if (!(flags & FLAG_FILE_EXTENSION))
			free(mime);
	}
}
