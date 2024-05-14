#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <string.h>

#ifndef SERVER_MAXBYTE
#define SERVER_MAXBYTE 1024
#endif

// Default server IP address and port number
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 9999

// Ŭ���̾�Ʈ �ʱ�ȭ �Լ� ����
SOCKET client_init(char* ip, int port);

// ä�� ���� �Լ� ����
unsigned int WINAPI do_chat_service(void* params);

int main(int argc, char* argv[])
{
    char ip_addr[256] = DEFAULT_IP; // Default server IP address
    int port_number = DEFAULT_PORT; // Default port number
    char nickname[50] = ""; // Buffer for nickname
    unsigned int tid;
    SOCKET sock;
    char input[SERVER_MAXBYTE] = "";
    char message[SERVER_MAXBYTE] = "";
    char* pexit = NULL;
    HANDLE mainthread;

    // ����� �г��� �Է�
    if (argc < 2)
    {
        printf("\nUsage: client [nickname]\n\n");
        printf("       e.g., client.exe mainCodes\n");
        exit(0);
    }

    if (argv[1] != NULL)
    {
        strcpy_s(nickname, sizeof(nickname), argv[1]);
    }

    // Ŭ���̾�Ʈ �ʱ�ȭ
    sock = client_init(ip_addr, port_number);
    if (sock == INVALID_SOCKET)
    {
        printf("Error initializing the client\n");
        exit(0);
    }

    // ä�� ���� ������ ����
    mainthread = (HANDLE)_beginthreadex(NULL, 0, do_chat_service, (void*)sock, 0, &tid);
    if (mainthread)
    {
        // ����� �Է� �ޱ� �� �޽��� ����
        while (1)
        {

            fgets(input, SERVER_MAXBYTE, stdin);
            if (strcmp(input, "/x\n") == 0) // �Է��� "/x"�� �� ����
                break;

            sprintf(message, "[%s] %s", nickname, input);
            send(sock, message, strlen(message), 0);
        }

        // Ŭ���̾�Ʈ ���� ó��
        closesocket(sock);
        CloseHandle(mainthread);
    }

    return 0;
}

// Ŭ���̾�Ʈ �ʱ�ȭ �Լ� ����
SOCKET client_init(char* ip, int port)
{
    SOCKET server_socket;
    WSADATA wsadata;
    SOCKADDR_IN server_address = { 0 };

    // Winsock �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        printf("Error initializing Winsock\n");
        return INVALID_SOCKET;
    }

    // ���� ����
    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Socket error\n");
        WSACleanup(); // ���� ������ ������ ��� Winsock�� �����ؾ� �մϴ�.
        return INVALID_SOCKET;
    }

    // ���� �ּ� ����
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    // ������ ����
    if (connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        printf("Error connecting to the server\n");
        closesocket(server_socket); // ���� ���ῡ ������ ��� ������ �ݾƾ� �մϴ�.
        WSACleanup(); // Winsock�� �����ؾ� �մϴ�.
        return INVALID_SOCKET;
    }

    return server_socket;
}

// ä�� ���� �Լ� ����
unsigned int WINAPI do_chat_service(void* params)
{
    SOCKET s = (SOCKET)params;
    char recv_message[SERVER_MAXBYTE];
    WSANETWORKEVENTS ev;
    HANDLE event = WSACreateEvent();

    // ���Ͽ� �̺�Ʈ ��� (���� �� ����)
    WSAEventSelect(s, event, FD_READ | FD_CLOSE);

    while (1)
    {
        // �̺�Ʈ ���
        if (WSAWaitForMultipleEvents(1, &event, FALSE, WSA_INFINITE, FALSE) != WSA_WAIT_EVENT_0)
            continue;

        WSAEnumNetworkEvents(s, event, &ev);
        if (ev.lNetworkEvents & FD_READ)
        {
            // �޽��� ����
            int len = recv(s, recv_message, SERVER_MAXBYTE - 1, 0);
            if (len > 0)
            {
                recv_message[len] = '\0'; // ���ŵ� �����͸� ���ڿ��� ����� ���� NULL ���� �߰�
                printf("%s\n", recv_message);
            }
        }
        else if (ev.lNetworkEvents & FD_CLOSE)
        {
            // ���� ���� �޽��� ��� �� ���� ����
            printf("The server service has been terminated (exit: \"/x\")\n");
            break;
        }
    }

    // �̺�Ʈ ��ü ����
    WSACloseEvent(event);

    return 0;
}
