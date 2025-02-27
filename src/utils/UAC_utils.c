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
INT RelaunchWithElevation(int argc, char *argv[]) {
    wchar_t szPath[MAX_PATH];
    if (!GetModuleFileName(NULL, szPath, MAX_PATH)) {
        printf("GetModuleFileName failed (%d).\n", GetLastError());
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    // 获取子进程pid
    DWORD pid;
    pid = GetProcessId(sei.hProcess);
    if (!pid) {
        printf("GetProcessId failed (%d).\n", GetLastError());
    }

    // 等待新进程结束
    WaitForSingleObject(sei.hProcess, INFINITE);
    // 移动到文件末尾
    fseek(stdout, 0, SEEK_END); 
    fseek(stderr, 0, SEEK_END); 

    // 获取新进程的返回值
    DWORD exitCode;
    if (!GetExitCodeProcess(sei.hProcess, &exitCode)) {
        printf("GetExitCodeProcess failed (%d).\n", GetLastError());
        CloseHandle(sei.hProcess);
        return EXIT_FAILURE;
    }
    printf("[subprocess pid=%d], [exitCode=%d]\n", pid, exitCode);

    CloseHandle(sei.hProcess);

    // 判断新进程的返回值
    return exitCode;
}

#undef TEMPWSTR_LENGTH
