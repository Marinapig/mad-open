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
#include "cache.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		return EXIT_FAILURE;
	int ch;
	static bool foundc = false;
	while ((ch = getopt(argc, argv, "c:")) != -1)
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
			magic_init(optarg);
			foundc = true;
		}
	}
	#ifdef USE_CACHE
	bool havecache = shouldCache();
	if (!foundc)
	{
		if (!havecache)
		{
			puts("Didnt use cache");
			magic_init(NULL);
		}
		else
		{
			puts("Used cache");
			cache_read();
		}
	}
	if (!havecache)
		cache_build();

	xdgDirs_clear();
	#else
	if (!foundc)
		magic_init(NULL);
	#endif

	Association *found = magic_getassociation(argv[optind]);
	if (found)
	{
		bool shouldfork = ( strstr(found->mime, "text/") ) == NULL; //Maybe this should be specifiable in the rules file
		char *args[] = { found->program, argv[optind] , 0};
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
}
