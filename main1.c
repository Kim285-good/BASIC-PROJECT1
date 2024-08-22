#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mysql.h>
#include "database.h"
#include "webcrawler.h"
#include "schedule.h"
#include "chat.h"

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "dh01030522"
#define DATABASE_USERS "users"
#define DATABASE_SCHEDULE "schedule"

void startChattingService();

void mainMenu(MYSQL* conn, const char* username) {
    while (1) {
        system("cls");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\t========================================================\n");
        printf("                                                ");
        printf("1. View News Title\t   \n");
        printf("                                                ");
        printf("2. View today's weather   \n");
        printf("                                                ");
        printf("3. Manage Schedule\t   \n");
        printf("                                                ");
        printf("4. Chatting Service\t   \n");
        printf("                                                ");
        printf("5. Logout\t\t   \n");
        printf("                                                ========================================================\n");
        printf("\t\t\t\t\t\tEnter your choice:");
        int option;
        scanf("%d", &option);

        if (option == 1) {
            int select = 0;
            char str1[5];
            const char* url = "https://news.naver.com/section/";
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\n");
            printf("\t\t\t\t\t\t========================================================\n");
            printf("                                                ");
            printf("1. Politics\t\t   \n");
            printf("                                                ");
            printf("2. Economy\t\t   \n");
            printf("                                                ");
            printf("3. Society\t\t   \n");
            printf("                                                ");
            printf("4. Lifestyle/Culture\t   \n");
            printf("                                                ");
            printf("5. World\t\t   \n");
            printf("                                                ");
            printf("5. IT/Science\t\t   \n");
            printf("                                                ========================================================\n");
            printf("\t\t\t\t\t\tEnter your choice:");
            scanf("%d", &select);
            if (select == 1) strcpy(str1, "100");
            else if (select == 2) strcpy(str1, "101");
            else if (select == 3) strcpy(str1, "102");
            else if (select == 4) strcpy(str1, "103");
            else if (select == 5) strcpy(str1, "104");
            else if (select == 6) strcpy(str1, "105");

            char new_url[100];
            strcpy(new_url, url);
            strcat(new_url, str1);
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\n");
            crawl_webpage(new_url);
            Sleep(5000);
        }
        else if (option == 2) {
            const char* url = "https://search.daum.net/search?w=tot&DA=YZR&t__nil_searchbox=btn&q=%EC%84%9C%EC%9A%B8%ED%8A%B9%EB%B3%84%EC%8B%9C+%EB%82%A0%EC%94%A8";
            crawl_weather(url);
        }
        else if (option == 3) {
            manageSchedule(conn, username);
        }
        else if (option == 4) {
            startChattingService();
        }
        else if (option == 5) {
            break; // Logout and return to main menu
        }
        else {
            printf("Invalid choice!\n");
        }
    }
}

int main() {
    system("mode con cols=100 lines=50");
    SetConsoleOutputCP(CP_UTF8);

    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("mysql_init failed\n");
        exit(1);
    }

    if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE_USERS, 0, NULL, 0)) {
        printf("Error connecting to database: %s\n", mysql_error(conn));
        exit(1);
    }

    while (1) {
        system("cls");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\t========================================================\n");
        printf("                                                ");
        printf("\t1. Login\t   \n");
        printf("                                                ");
        printf("\t2. Register   \t   \n");
        printf("                                                ========================================================\n");

        printf("\t\t\t\t\t\tEnter your choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            char username[255];
            char password[255];
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\n");
            printf("\t\t\t\t\t\t\n");
            printf("                                                ========================================================\n");
            printf("\t                                                                  login tab\t   \n");
            printf("                                                ========================================================\n");
            printf("\t\t\t\t\t\tEnter username: ");
            scanf("%s", username);
            printf("\t\t\t\t\t\tEnter password: ");
            scanf("%s", password);

            if (login(conn, username, password)) {
                system("cls");
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tLogin successful! Welcome %s!\n", username);
                Sleep(2000);
                mainMenu(conn, username);
            }
            else {
                system("cls");
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tLogin failed!\n");
                Sleep(2000);
            }
        }
        else if (choice == 2) {
            registerUser(conn);
        }
        else {
            printf("Invalid choice!\n");
        }
    }

    mysql_close(conn);
    return 0;
}





