#ifndef UTILS_H
#define UTILS_H

#include <wchar.h>

int file_exists(const char *filename);

wchar_t *convertCharToWChar(const char* message);

#define WCharChar(x) (convertCharToWChar(x))

#define free2NULL(x) free(x);x=NULL;

#endif // UTILS_H