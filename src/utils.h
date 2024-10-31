#ifndef UTILS_H
#define UTILS_H

#define UNICODE
#define _UNICODE

#include <wchar.h>
#include <windows.h>

int file_exists(const char *filename);

wchar_t *convertCharToWChar(const char* message);

#define WCharChar(x) (convertCharToWChar(x))

#define free2NULL(x) free(x);x=NULL;

void printWCharFromCharAndShow(const char *para_str, wchar_t *para_pointer_wstr, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag, int para_flag_free_wstr);

void printWCharAndShow(const wchar_t *para_str, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag);

#endif // UTILS_H