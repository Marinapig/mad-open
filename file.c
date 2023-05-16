#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <pwd.h>

#include <linux/limits.h>

#include <sys/stat.h>

#include <xdgdirs.h>

char *getConfigFile(bool cache)
{
	const char *s;
	if (cache)
		s = xdgCacheHome();
	else
		s = xdgConfigHome();
	char *buf = malloc(PATH_MAX);
	if (!buf)
		exit(EXIT_FAILURE);
	snprintf(buf, PATH_MAX, "%s/mad-open/%s", s, cache ? "cache" : "rules");
	return buf;
}
