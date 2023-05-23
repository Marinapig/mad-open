#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>

#include <basedir.h>

char *getConfigFile(void)
{
	char *buf = malloc(PATH_MAX);
	if (!buf)
		exit(EXIT_FAILURE);
	xdgHandle handle;
	xdgInitHandle(&handle);
	snprintf(buf, PATH_MAX, "%s/mad-open/rules", xdgConfigHome(&handle));
	xdgWipeHandle(&handle);
	return buf;
}

char *getFileExt(const char *s)
{
	return strrchr(s, '.');
}
