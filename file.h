#ifndef FILE_H
#define FILE_H
#include <stdbool.h>
char *getConfigFile(bool cache);
int file_creationTime(const char *path);
#endif
