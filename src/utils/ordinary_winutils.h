#pragma once

#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include <winnls.h>
#include <unistd.h>
#include "ordinaries.h"


int file_exists(const char *filename);

wchar_t *convertCharToWChar(const char* message);

#define WCharChar(x) (convertCharToWChar(x))


void printWCharFromCharAndShow(const char *para_str, wchar_t *para_pointer_wstr, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag, int para_flag_free_wstr_message);

void printCharAndShow(const char *para_str, wchar_t *para_pointer_wstr, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag);

void printWCharAndShow(const wchar_t *para_str, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag);

#define getDefaultCodePage() (GetACP())

UINT getLocaleCodePage();

BOOL checkIfCurrentOutputCodePageIsDefault();

BOOL checkIfLocaleCodePageMatchCurrentOutputCodePage();

