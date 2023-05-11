#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pwd.h>

char *getConfigFile(void)
{
	char *buf = malloc(PATH_MAX);
	if (!buf)
		return 0;
	char *dir = getenv("XDG_CONFIG_HOME");
	if (dir)
	{
		snprintf(buf, PATH_MAX, "%s/mad-open/rules", dir);
		strncpy(buf, dir, PATH_MAX);
		return buf;
	}

	dir = getenv("HOME");
	if (dir)
	{
		snprintf(buf, PATH_MAX, "%s/.config/mad-open/rules", dir);
		return buf;
	}

        struct passwd *pw = getpwuid(getuid());
	if (pw)
	{
		snprintf(buf, PATH_MAX, "%s/.config/mad-open/rules", pw->pw_dir);
		return buf;
	}

	perror("Couldn't find a home directory AT ALL");
	free(buf);
	exit(EXIT_FAILURE);
}
