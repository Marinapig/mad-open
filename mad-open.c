#include <stdio.h>
#include <stdlib.h>
#include <magic.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <fcntl.h>

#include <xdgdirs.h>

#include "file.h"
#include "char.h"
#include "magic.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		return EXIT_FAILURE;
	int ch;
	bool foundc = false;
	bool foundt = false;
	char *cval = 0;
	while ((ch = getopt(argc, argv, "tc:")) != -1)
	{
		switch (ch)
		{
			case 'c': 
			if (foundc)
				break;
			if (!optarg || *optarg == ':')
			{
				perror("");
				return EXIT_FAILURE;
			}
			cval = calloc(strlen(optarg) + 1, sizeof(char) );
			strcpy(cval, optarg);
			foundc = true;
			break;
			case 't':
			foundt = true;
			break;
		}
	}
	char *mime = get_mimetype(argv[optind], foundt);
	if (!mime)
	{
		perror("Couldn't get mimetype");
		exit(EXIT_FAILURE);
	}
	char *filename;
	if (foundc)
		filename = cval;
	else
		filename = getConfigFile(false);

	if (!filename)
	{
		free(mime);
		perror("Couldn't get rules filename");
		exit(EXIT_FAILURE);
	}
	Association found;
	bool didfind = magic_grep(filename, mime, &found);
	free(filename);
	free(mime);
	xdgDirs_clear();
	if (didfind)
	{
		char *args[] = { found.program, argv[optind] , 0};
		#ifndef NDEBUG
		if (!found.nofork)
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
}
