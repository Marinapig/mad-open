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
void magic_init(char *s);
char *get_mimetype(const char *filename, bool text_generic);
bool magic_grep(const char *filename, char *mime, Association *rule);
#endif
