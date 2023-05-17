#include <stdio.h>
#include <stdlib.h>

#include <linux/limits.h>

#include <xdgdirs.h>

char *getConfigFile(void)
{
	char *buf = malloc(PATH_MAX);
	if (!buf)
		exit(EXIT_FAILURE);
	snprintf(buf, PATH_MAX, "%s/mad-open/%s", xdgConfigHome(), "rules");
	xdgDirs_clear();
	return buf;
}
