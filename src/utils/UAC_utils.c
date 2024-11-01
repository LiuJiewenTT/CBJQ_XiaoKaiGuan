#include "UAC_utils.h"

#define TEMPWSTR_LENGTH 1024
extern int flag_unhide;


// 检查当前进程是不是以管理员权限运行
BOOL IsRunAsAdmin() {
    BOOL isAdmin = FALSE;
    HANDLE hToken = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD size;
        
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
            isAdmin = elevation.TokenIsElevated;
        }

        CloseHandle(hToken);
    }

    return isAdmin;
}


// 以提升权限运行当前进程
BOOL RelaunchWithElevation(int argc, char *argv[]) {
    wchar_t szPath[MAX_PATH];
    if (!GetModuleFileName(NULL, szPath, MAX_PATH)) {
        printf("GetModuleFileName failed (%d).\n", GetLastError());
        return FALSE;
    }

    // 构建命令行参数
    wchar_t cmdLine[TEMPWSTR_LENGTH] = L"";
    // wcscat(cmdLine, L"\"");
    // wcscat(cmdLine, szPath);
    // wcscat(cmdLine, L"\"");

    for (int i = 1; i < argc; ++i) {
        wcscat(cmdLine, L" ");
        wcscat(cmdLine, L"\"");
        wchar_t *arg = WCharChar(argv[i]);
        wcscat(cmdLine, arg);
        wcscat(cmdLine, L"\"");
        free2NULL(arg);
    }

    wprintf(L"relaunch args=%s\n", cmdLine);

    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = szPath;
    sei.lpParameters = cmdLine; 
    if( flag_unhide ){
        sei.nShow = SW_SHOWNORMAL;
    }
    else {
        sei.nShow = SW_HIDE; // 不显示窗口
    }
    sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE | SEE_MASK_NOASYNC; 
    // SEE_MASK_NOCLOSEPROCESS保持进程句柄

    if (!ShellExecuteEx(&sei)) {
        printf("ShellExecuteEx failed (%d).\n", GetLastError());
        return FALSE;
    }

    // return TRUE;

    // 等待新进程结束
    WaitForSingleObject(sei.hProcess, INFINITE);

    // 获取新进程的返回值
    DWORD exitCode;
    if (!GetExitCodeProcess(sei.hProcess, &exitCode)) {
        printf("GetExitCodeProcess failed (%d).\n", GetLastError());
        CloseHandle(sei.hProcess);
        return FALSE;
    }

    CloseHandle(sei.hProcess);

    // 判断新进程的返回值
    if (exitCode == EXIT_SUCCESS) {
        return TRUE;
    } else {
        return FALSE;
    }
}

#undef TEMPWSTR_LENGTH
