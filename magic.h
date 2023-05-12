#ifndef MAGIC_H
#define MAGIC_H
#include <stdbool.h>
#define MIME_LEN 64
#define PROG_LEN 24
typedef struct
{
	char mime[MIME_LEN];
	char program[PROG_LEN];
	bool wildcard;
	bool nofork;
} Association;
static Association* magic_which(const char *s);
void magic_init(char *s);
Association* magic_getassociation(const char *s, bool text_generic);
#endif
