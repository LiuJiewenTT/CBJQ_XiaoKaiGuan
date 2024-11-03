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


// 获取当前用户的 SID
BOOL GetCurrentUserSid(PSID* ppSid) {
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        return FALSE;
    }

    DWORD dwSize = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
    PTOKEN_USER ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
    if (!ptu || !GetTokenInformation(hToken, TokenUser, ptu, dwSize, &dwSize)) {
        if (ptu) {
            HeapFree(GetProcessHeap(), 0, ptu);
        }
        CloseHandle(hToken);
        return FALSE;
    }

    // 分配 SID 的内存并复制
    DWORD sidSize = GetSidLengthRequired(((SID*)(ptu->User.Sid))->SubAuthorityCount);
    *ppSid = (PSID)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sidSize);
    if (*ppSid) {
        CopySid(sidSize, *ppSid, ptu->User.Sid);
    }

    HeapFree(GetProcessHeap(), 0, ptu);
    CloseHandle(hToken);
    return TRUE;
}

#define MAX_TOKEN_USER_SIZE (sizeof(TOKEN_USER) + MAX_SID_SIZE)

// 检查用户是否有相应进程是否正在运行，NULL用户SID表示检查所有用户的进程
BOOL IsProcessRunning(const wchar_t* processName, PSID pCurrentUserSid, int *countResult) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    int cnt = 0;
    BOOL found = FALSE;

    HANDLE hHeap = NULL;
    PTOKEN_USER ptu = NULL;
    HANDLE hProcess = NULL;
    HANDLE hToken = NULL;
    DWORD dwSize = 0;
    if (pCurrentUserSid != NULL) {
        // 复用堆句柄
        hHeap = GetProcessHeap();
        ptu = (PTOKEN_USER)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, MAX_TOKEN_USER_SIZE);
    }
    
    if (Process32First(hSnapshot, &pe)) {
        do {
            if ( pCurrentUserSid == NULL ) {
                if (wcscmp(processName, pe.szExeFile) == 0) {
                    // CloseHandle(hSnapshot);
                    ++cnt;
                    found = TRUE;
                }
            } else {
                if (wcscmp(processName, pe.szExeFile) == 0) {
                    // 获取进程句柄
                    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
                    if (hProcess) {
                        // 获取进程的拥有者 SID
                        hToken = NULL;
                        if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                            dwSize = 0;
                            GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
                            if (GetTokenInformation(hToken, TokenUser, ptu, dwSize, &dwSize)) {
                                // 比较 SID
                                if (EqualSid(pCurrentUserSid, ptu->User.Sid)) {
                                    ++cnt;
                                    found = TRUE;
                                }
                            }
                            CloseHandle(hToken);
                        }
                        CloseHandle(hProcess);
                    }
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    if (ptu) {
        HeapFree(hHeap, 0, ptu);
    }
    CloseHandle(hSnapshot);
    if (countResult) {
        *countResult = cnt;
    }
    return found;
}


// 检查用户是否有相应进程是否正在运行
BOOL IsProcessRunning_User(const wchar_t* processName) {
    PSID pCurrentUserSid = NULL;
    if (!GetCurrentUserSid(&pCurrentUserSid)) {
        printf("Error getting current user SID.\n");
        return FALSE;
    }

    BOOL result = IsProcessRunning(processName, pCurrentUserSid, NULL);
    if (pCurrentUserSid) {
        FreeSid(pCurrentUserSid);
    }
    return result;
}


INT CountProcessRunning_Global(const wchar_t* processName) {
    INT cnt = 0;
    IsProcessRunning(processName, NULL, &cnt);
    return cnt;
}


INT CountProcessRunning_User(const wchar_t* processName) {
    PSID pCurrentUserSid = NULL;
    if (!GetCurrentUserSid(&pCurrentUserSid)) {
        printf("Error getting current user SID.\n");
        return -1;
    }

    INT cnt = 0;
    IsProcessRunning(processName, pCurrentUserSid, &cnt);
    if (pCurrentUserSid) {
        FreeSid(pCurrentUserSid);
    }
    return cnt;
}


// 重定向标准输出到文件
BOOL RedirectOutput(const char* outputFilePath, BOOL append, FILE* output) {
    if (outputFilePath == NULL) {
        printf("Output file path cannot be NULL.\n");
        return FALSE;
    }

    // 共享文件，避免主进程和子进程产生冲突
    FILE* file = _fsopen(outputFilePath, append ? "a" : "w", _SH_DENYNO);
    if (file == NULL) {
        printf("Error opening file.\n");
        return FALSE;
    }
    
    // 重定向输出到文件
    if (_dup2(_fileno(file), _fileno(output)) != 0) {
        printf("Error redirecting output.\n");
        // CloseHandle(hFile);
        return FALSE;
    }

    return TRUE;
}


