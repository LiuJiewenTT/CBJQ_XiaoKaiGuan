#include "utils.h"
#include <winnls.h>
#include <stdio.h>
#include <unistd.h>


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
    para_pointer_wstr = convertCharToWChar(para_str);
    printf("debug: %s\n", para_str);
    printWCharAndShow(para_pointer_wstr, para_MessageBox_Title, para_MessageBox_flag);

    if( para_flag_free_wstr_message == TRUE ){
        free2NULL(para_pointer_wstr);
    }
    return;
}


void printWCharAndShow(const wchar_t *para_str, const wchar_t *para_MessageBox_Title, int para_MessageBox_flag){
    wprintf(L"%ls\n", para_str);
    MessageBox(NULL, para_str, para_MessageBox_Title, para_MessageBox_flag);
    return;
}

