#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> // perror 함수를 사용하기 위해 추가
#include <mysql.h>
#include "schedule.h"

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "dh01030522"
#define DATABASE_USERS "users"
#define DATABASE_SCHEDULE "schedule"

void manageSchedule(MYSQL* conn, const char* username) {
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t\t=========================================================\n");
    printf("                                                ");
    printf("Welcome to the schedule management program, %s!  \n", username);
    printf("                                                ");
    printf("1. Add Schedule\t\t\t\t    \n");
    printf("                                                ");
    printf("2. View Schedule\t\t\t\t    \n");
    printf("                                                =========================================================\n");
    printf("                                                 ");
    printf("Enter your choice:");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        char date[20];
        char schedule[255];

        printf("                                                 ");
        printf("Enter date (YYYY-MM-DD): ");
        scanf("%s", date);
        printf("                                                 ");
        printf("Enter schedule: ");
        scanf(" %[^\n]", schedule);

        char query[1000];
        sprintf(query, "INSERT INTO schedule.schedules (username, date, schedule) VALUES ('%s', '%s', '%s')", username, date, schedule);

        if (mysql_query(conn, query)) {
            perror("MySQL 쿼리 오류");
            exit(1); // 오류를 적절히 처리하세요
        }

        system("cls");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tSchedule added successfully!\n");
        Sleep(2000);
    }
    else if (choice == 2) {
        char date[20];
        system("cls");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("                                             ");
        printf("Enter date to view schedule (YYYY-MM-DD): ");
        scanf("%s", date);

        char query[1000];
        sprintf(query, "SELECT schedule FROM schedule.schedules WHERE username='%s' AND date='%s'", username, date);

        if (mysql_query(conn, query)) {
            perror("MySQL 쿼리 오류");
            exit(1); // 오류를 적절히 처리하세요
        }

        MYSQL_RES* result = mysql_store_result(conn);

        if (result == NULL) {
            perror("MySQL 쿼리 결과 오류");
            exit(1); // 오류를 적절히 처리하세요
        }

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;

        system("cls");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\t========================================================\n");
        printf("                                                ");
        printf("Schedule for %s\t   \n", date);
        printf("                                                ");

        while ((row = mysql_fetch_row(result))) {
            printf("%s\n", row[0]);
        }

        mysql_free_result(result);
        printf("                                                ========================================================\n");
        Sleep(3000);
    }
    else {
        printf("Invalid choice!\n");
    }
}
