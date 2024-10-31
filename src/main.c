#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include "utils.h"
#include "version.rc"


#define TEMPSTR_LENGTH 2048
#define TEMPWSTR_LENGTH 2048


int flag_unhide = 0;
int flag_need_UAC_start = 0;


// 函数声明
BOOL IsRunAsAdmin();
BOOL RelaunchWithElevation(int argc, char *argv[]);


int main(int argc, char **argv) {
    printf("arg[0]=%s\n", argv[0]);

    wchar_t *pw1 = NULL;
    wchar_t *pw2 = NULL;
    char tempstr1[TEMPSTR_LENGTH];
    char tempstr2[TEMPSTR_LENGTH];
    char tempstr3[TEMPSTR_LENGTH];
    char tempstr4[TEMPSTR_LENGTH];
    char tempstr5[TEMPSTR_LENGTH];
    int ret = 0;

    int var_localization = -1;
    int var_localization_new = -1;

    sprintf(tempstr1, "%s.unhide", argv[0]);
    flag_unhide = file_exists(tempstr1);

    pw2 = WCharChar(PROGRAM_NAME_PRETTY);

    // 检查自身UAC授权情况
    if (IsRunAsAdmin()) {
        printf("This process has UAC authorization (Run as Administrator).\n");
    } else {
        printf("This process does not have UAC authorization.\n");
        printf("Relaunching with UAC request.\n");
        if( RelaunchWithElevation(argc, argv) ){
            printf("Relaunched process with elevated privileges worked well.\n");
            return EXIT_SUCCESS;
        }
        else {
            printf("Failed to relaunch with elevated privileges or relaunched process returned failed.\n");
            return EXIT_FAILURE;
        }
    }

    // 读取文件内容
    FILE *fp_localization = NULL;
    fp_localization = fopen("localization.txt", "r");

    if ( fp_localization == NULL ) {
        printf("Failed to open localization.txt.\n");
        return EXIT_FAILURE;
    }
    memset(tempstr1, 0, TEMPSTR_LENGTH);
    fgets(tempstr1, TEMPSTR_LENGTH, fp_localization);
    printf("localization.txt 的内容: %s\n", tempstr1);
    fclose(fp_localization);

    // 第一版，不做校验。后续应该会添加内容校验功能
    sprintf(tempstr4, "开启");
    sprintf(tempstr5, "关闭");
    #define mfunc(x) ( x == 0 ? tempstr5 : tempstr4 )
    sscanf(tempstr1, "localization = %d", &var_localization);
    var_localization_new = ( var_localization == 0 ? 1 : 0 );
    sprintf(tempstr2, "localization = %d", var_localization_new );
    sprintf(tempstr3, "请确认：\n原：%s (识别结果：%s)\n新：%s (识别结果：%s)", tempstr1, mfunc(var_localization), tempstr2, mfunc(var_localization_new));   
    printf("%s\n", tempstr3);
    #undef mfunc
   
    // 弹出确认对话框
    pw1 = WCharChar(tempstr3);
    ret = MessageBox(NULL, pw1, pw2, MB_YESNO | MB_ICONQUESTION);
    free2NULL(pw1);

    if ( ret == IDNO ) {
        printf("User canceled.\n");
        return EXIT_FAILURE;
    }

    // 写入内容
    fp_localization = fopen("localization.txt", "w");
    if ( fp_localization == NULL ) {
        sprintf(tempstr1, "无法打开文件 localization.txt\n");
        printf("%s", tempstr1);
        pw1 = WCharChar(tempstr1);
        MessageBox(NULL, pw1, pw2, MB_OK | MB_ICONERROR);
        free2NULL(pw1);
        return EXIT_FAILURE;
    }
    fputs(tempstr2, fp_localization);
    fclose(fp_localization);

    sprintf(tempstr1, "成功完成开关状态切换！\n");
    pw1 = WCharChar(tempstr1);
    MessageBox(NULL, pw1, pw2, MB_OK | MB_ICONINFORMATION);
    free2NULL(pw1);
    free2NULL(pw2);

    return 0;
}


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
