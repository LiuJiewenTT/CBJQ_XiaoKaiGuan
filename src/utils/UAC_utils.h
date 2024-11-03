#pragma once

#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include "ordinaries.h"
#include "ordinary_winutils.h"

// 函数声明
BOOL IsRunAsAdmin();
INT RelaunchWithElevation(int argc, char *argv[]);

