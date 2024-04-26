#include <WINDOWS.H> // Windows ��� ����
#include <wininet.h> // WinINet API ��� ����
#include <stdio.h> // ǥ�� ����� ��� ����
#include <stdlib.h> // ǥ�� ���̺귯�� ��� ����
#include <string.h> // ���ڿ� ó�� ��� ����

#define BUFFER_SIZE 4096 // ���� ũ�� ��� ����

// ��� ������ �������� �Լ� ����
void GetArticleTitles(const char* url);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    char url[] = "https://news.daum.net/"; // ��� URL

    // ��� ������ �����ͼ� ���
    GetArticleTitles(url);

    return 0;
}

// ��� ������ �������� �Լ� ����
void GetArticleTitles(const char* url) {
    // ���ͳ� ������ ���� ���� �ڵ� ����
    HINTERNET h_internet = InternetOpenA("NewsCrawler", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (h_internet == NULL) { // ���� ���� ��
        printf("���ͳ� ���� ���⿡ �����߽��ϴ�.\n");
        return;
    }

    // URL�� �����ϱ� ���� �ڵ� ����
    HINTERNET h_connect = InternetOpenUrlA(h_internet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (h_connect == NULL) { // ���� ���� ��
        printf("URL�� �����ϴ� �� �����߽��ϴ�.\n");
        InternetCloseHandle(h_internet);
        return;
    }

    char buffer[BUFFER_SIZE]; // �����͸� ���� ����
    DWORD read_byte = 0; // ���� ����Ʈ ��
    if (InternetReadFile(h_connect, buffer, BUFFER_SIZE - 1, &read_byte)) { // ������ �б� �õ�
        buffer[read_byte] = '\0'; // ���ڿ� ���Ḧ ���� �� ���� �߰�

        // HTML���� ��� ������ ã��
        char* p_title_start = strstr(buffer, "<title>");
        if (p_title_start != NULL) { // ���� �±� ã��
            p_title_start += strlen("<title>"); // ���� �±� ���̸�ŭ ������ �̵�
            for (int i = 0; i < 5; ++i) { // ���� 5���� ��� ���� ���
                char* p_title_end = strstr(p_title_start, "</title>"); // �� �±� ã��
                if (p_title_end != NULL) { // �� �±� ã��
                    *p_title_end = '\0'; // �� �±� ��ġ�� �� ���� �߰��Ͽ� ���ڿ� ����
                    printf("%d: %s\n", i + 1, p_title_start); // ��� ���� ���
                    p_title_start = strstr(p_title_end + strlen("</title>"), "<title>"); // ���� ��� ���� ���� ��ġ�� �̵�
                    if (p_title_start != NULL)
                        p_title_start += strlen("</title>");
                    else
                        break; // �� �̻� ��� ������ ������ �ݺ� ����
                }
                else {
                    printf("��� ������ ã�� �� �����ϴ�.\n");
                    break;
                }
            }
        }
        else {
            printf("��� ������ ã�� �� �����ϴ�.\n");
        }
    }


    InternetCloseHandle(h_connect);
    InternetCloseHandle(h_internet);
}
