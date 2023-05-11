#ifndef MAGIC_H
#define MAGIC_H
#include <stdbool.h>
typedef struct
{
	char mime[24];
	char program[24];
	bool wildcard;
} Association;
static Association* magic_which(const char *s);
void magic_init(char *s);
Association* magic_getassociation(const char *s);
#endif
