#ifndef MAGIC_H
#define MAGIC_H
#include <stdbool.h>
typedef struct
{
	char mime[255];
	char program[255];
	bool nofork;
} Association;
void magic_init(char *s);
char *get_mimetype(const char *filename, bool text_generic);
bool magic_grep(const char *filename, const char *mime, Association *rule);
#endif
