#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void crawl_webpage(const char* url) {
    HINTERNET hInternet = NULL, hConnect = NULL;
    char buffer[4096];
    DWORD bytesRead;
    char* end_tag;
    int temp = 0,temp1=0; // ���� ��µ� �����⶧���� �װ��� �������� �ӽú���
    int count = 0;// ǳ�Ӹ��� ������ ã������ ī��Ʈ
    hInternet = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("WinINet �ʱ�ȭ�� �����߽��ϴ�: %d\n", GetLastError());
        return;
    }

    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("URL�� ������ �� �����ϴ�: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return;
    }

    char current_temp[10] = { 0 };
    char dust_status[20] = { 0 };
    char water_status[20] = { 0 };
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        // "<strong class=\"txt_temp\">" �±� ã��
        char* start_tag = strstr(buffer, "<strong class=\"txt_temp\">");

        //printf("%s", buffer);
        if (start_tag != NULL) {
            // "</strong>" �±� ã��
             end_tag= strstr(start_tag, "</strong>");
            if (end_tag != NULL) {
                
                for (char* ptr = start_tag + strlen("<strong class=\"txt_temp\">"); ptr < end_tag; ++ptr) {
                    if (*ptr >= '0' && *ptr <= '9' && temp != 2) {
                        strncat(current_temp, ptr, 1);
                        temp++;
                    }
                }
            }
        }
        
        start_tag = strstr(buffer, "<dd class=\"dust_type1\">");
        
        if (start_tag != NULL) { 
            end_tag = strstr(start_tag, "</dd>");
            if (end_tag != NULL) { 
                for (char* ptr = start_tag + strlen("<dd class=\"dust_type1\">"); ptr < end_tag; ++ptr) {
                    strncat(dust_status, ptr, 1);
                }
            }
        }
        start_tag = strstr(buffer, "<span class=\"txt_tit\">");
        
        while (start_tag != NULL && count <1) {
            count++;
            start_tag = strstr(start_tag+1 , "<span class=\"txt_tit\">");
        }      
        if (start_tag != NULL) {
            end_tag = strstr(start_tag, "</dd>");
            if (end_tag != NULL) {
                for (char* ptr = start_tag + strlen("</span>"); ptr < end_tag; ++ptr) {
                    if (*ptr >= '0' && *ptr <= '9' && temp1!=2) {
                        strncat(water_status, ptr, 1);
                        temp1++;                       
                    }
                    
                }
                
            }
        }
    }

    printf("temp: %sC\n", current_temp);
    printf("dust: %s\n", dust_status);
    printf("water: %s%%", water_status);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    const char* url = "https://search.daum.net/search?w=tot&DA=YZR&t__nil_searchbox=btn&q=%EC%84%9C%EC%9A%B8%ED%8A%B9%EB%B3%84%EC%8B%9C+%EB%82%A0%EC%94%A8";
    crawl_webpage(url);
    return 0;
}