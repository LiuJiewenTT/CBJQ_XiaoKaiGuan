#include "ordinary_winutils.h"


int file_exists(const char *filename) {
    return access(filename, F_OK) != -1;
}

wchar_t *convertCharToWChar(const char* message){
    // 将 char 字符串转换为 wchar_t 字符串
    int len = MultiByteToWideChar(CP_UTF8, 0, message, -1, NULL, 0);
    wchar_t* wMessage = (wchar_t*)malloc(len * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, message, -1, wMessage, len);
    return wMessage;
}


void printWCharFromCharAndShow(const char *para_str, wchar_t *para_pointer_wstr, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag, int para_flag_free_wstr_message){
    
    printCharAndShow(para_str, para_pointer_wstr, para_MessageBox_Title, para_MessageBox_flag);

    if( para_flag_free_wstr_message == TRUE ){
        free2NULL(para_pointer_wstr);
    }
    return;
}


void printCharAndShow(const char *para_str, wchar_t *para_pointer_wstr, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag){
    printf("%hs\n", para_str);  // 或%s也行
    para_pointer_wstr = convertCharToWChar(para_str);

    MessageBox(NULL, para_pointer_wstr, para_MessageBox_Title, para_MessageBox_flag);
    return;
}


void printWCharAndShow(const wchar_t *para_str, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag){
    wprintf(L"%ls\n", para_str);
    MessageBox(NULL, para_str, para_MessageBox_Title, para_MessageBox_flag);
    return;
}


UINT getLocaleCodePage() {
    WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
    WCHAR codePageStr[10];
    UINT codePage = 0;

    // 获取用户默认 locale 名称
    GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

    // 获取对应 locale 的代码页
    int result = GetLocaleInfoEx(localeName, LOCALE_IDEFAULTCODEPAGE, codePageStr, sizeof(codePageStr) / sizeof(WCHAR));

    // 获取当前控制台的活动代码页
    UINT currentCodePage = GetConsoleOutputCP();

    if (result > 0) {
        codePage = (UINT)_wtoi(codePageStr); // 转换为整数
        wprintf(L"Locale: %s\n", localeName);
        wprintf(L"Default Code Page: %u\n", codePage);
    } else {
        wprintf(L"Error getting locale information: %d\n", GetLastError());
    }
    return codePage;
}


// 检查当前输出代码页是否被修改过，还是不是默认的 ANSI 代码页
BOOL checkIfCurrentOutputCodePageIsDefault() {
    // 获取系统的 ANSI 代码页
    UINT defaultCodePage = GetACP();
    // 获取当前控制台的活动代码页
    UINT currentCodePage = GetConsoleOutputCP();
    
    // 输出当前状态
    printf("System default ANSI CodePage: %u\n", defaultCodePage);
    printf("Current console active CodePage: %u\n", currentCodePage);

    // 检查代码页和 locale
    if (defaultCodePage != currentCodePage) {
        // printf("Warning: Current console active CodePage is different from system default ANSI CodePage.\n");
        return TRUE;
    }
    return FALSE;
}


// 检查locale的代码页和当前输出代码页是否一致
BOOL checkIfLocaleCodePageMatchCurrentOutputCodePage() {
    UINT localeCodePage = getLocaleCodePage();
    UINT currentCodePage = GetConsoleOutputCP();

    if (localeCodePage == currentCodePage) {
        printf("Locale CodePage is consistent with current console active CodePage.\n");
        return TRUE;
    }
    return FALSE;
}


