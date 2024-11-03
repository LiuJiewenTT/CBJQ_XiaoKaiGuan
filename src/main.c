#define UNICODE
#define _UNICODE

#include <locale.h>
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include "utils/utils.h"
#include "program_info.h"


#define TEMPSTR_LENGTH 2048
#define TEMPWSTR_LENGTH 2048


int flag_unhide = 0;
// int flag_need_UAC_start = 0;



int main(int argc, char **argv) {
    // 关闭输出缓冲
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    
    printf("arg[0]=%s\n", argv[0]);

    HWND hwnd = GetConsoleWindow();

    wchar_t *pw1 = NULL;
    wchar_t *pw2 = NULL;
    char *p1 = NULL;
    char tempstr1[TEMPSTR_LENGTH];
    char tempstr2[TEMPSTR_LENGTH];
    char tempstr3[TEMPSTR_LENGTH];
    char tempstr4[TEMPSTR_LENGTH];
    char tempstr5[TEMPSTR_LENGTH];
    int ret = 0;
    int lastOutputCP = 0;
    int lastCP = 0;
    int process_global_cnt = 0;
    int process_user_cnt = 0;
    char path_delimeter= '\\';
    char process_name[MAX_PATH];
    char process_working_dir[MAX_PATH];
    wchar_t *pw_process_name = NULL;

    int var_localization = -1;
    int var_localization_new = -1;

    setlocale(LC_CTYPE, "C.UTF-8");

    if ( ( lastOutputCP = GetConsoleOutputCP() ) != CP_UTF8 ) {
        printf("Setting console output code page from (%d) to UTF-8.\n", lastOutputCP);
        SetConsoleOutputCP(CP_UTF8);
    }
    if ( ( lastCP = GetConsoleCP() ) != CP_UTF8 ) {
        printf("Setting console code page from (%d) to UTF-8.\n", lastCP);
        SetConsoleCP(CP_UTF8);
    }

    memset(process_name, 0, MAX_PATH);
    memset(process_working_dir, 0, MAX_PATH);

    // 获取程序文件名和程序所在目录。
    p1 = strrchr(argv[0], path_delimeter);
    if( p1 == NULL ){
        strcpy(process_name, argv[0]);
        sprintf(process_working_dir, ".\\");
    }
    else {
        strcpy(process_name, p1+1);
        strncpy(process_working_dir, argv[0], p1-argv[0]);
        process_working_dir[p1-argv[0]] = 0;
    }
    pw_process_name = WCharChar(process_name);

    sprintf(tempstr1, "%s.unhide", argv[0]);
    flag_unhide = file_exists(tempstr1);
    if ( !flag_unhide ) {
        // ShowWindow(hwnd, SW_HIDE);
        sprintf(tempstr1, "%s.log", argv[0]);
        pw1 = WCharChar(tempstr1);
        // freopen(tempstr1, "w", stdout);
        process_global_cnt = CountProcessRunning_Global(pw_process_name);
        ret = RedirectStdOutput(pw1, (process_global_cnt > 1));
        free2NULL(pw1);
        if ( ret == FALSE ) {
            printWCharFromCharAndShow("无法重定向输出。", pw1, pw2, MB_OK | MB_ICONERROR, TRUE);
            return EXIT_FAILURE;
        }
        printf("hide. [PID=%d].\n", getpid());
    }

    pw2 = WCharChar(PROGRAM_NAME_PRETTY);

    #define program_info_divider "----------------------------------------------------------------\n"
    printf("%s%hs\n%s", program_info_divider, PROGRAM_INFO_STRING, program_info_divider);
    #undef program_info_divider

    process_user_cnt = CountProcessRunning_User(pw_process_name);
    if ( process_user_cnt > 2 ) {
        // 展示程序信息
        printWCharFromCharAndShow(PROGRAM_INFO_STRING, pw1, pw2, MB_OK | MB_ICONINFORMATION, TRUE);
        return EXIT_SUCCESS;
    }

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
            // printf("Failed to relaunch with elevated privileges or relaunched process returned failed.\n");
            printWCharFromCharAndShow("请求管理员权限失败或运行后进程返回失败。", pw1, pw2, MB_OK | MB_ICONERROR, TRUE);
            return EXIT_FAILURE;
        }
    }

    // 读取文件内容
    FILE *fp_localization = NULL;
    fp_localization = fopen("localization.txt", "r");

    if ( fp_localization == NULL ) {
        // printf("Failed to open localization.txt.\n");
        printWCharFromCharAndShow("无法打开文件 localization.txt。", pw1, pw2, MB_OK | MB_ICONERROR, TRUE);
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
        // printf("User canceled.\n");
        printWCharFromCharAndShow("用户取消。", pw1, pw2, MB_OK | MB_ICONERROR, TRUE);
        return EXIT_FAILURE;
    }

    // 写入内容
    fp_localization = fopen("localization.txt", "w");
    if ( fp_localization == NULL ) {
        printWCharFromCharAndShow("无法打开文件 localization.txt。", pw1, pw2, MB_OK | MB_ICONERROR, TRUE);
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


