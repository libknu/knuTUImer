#include<curses.h>
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "client.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

extern int max_x, max_y, menu_bar;

struct calendarInfo get_calender_info(struct tm tm) {
    struct calendarInfo info;
    int month = tm.tm_mon;
    int year = tm.tm_year + 1900;

    //총 며칠인지 계산
    if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11) {
        info.number_of_days_in_month = 31;
    } else if (month == 1) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            info.number_of_days_in_month = 29;
        } else {
            info.number_of_days_in_month = 28;
        }
    } else {
        info.number_of_days_in_month = 30;
    }

    //무슨 요일부터 시작하는지 계산. 첫째날의 정보를 담은 새로운 tm인 first_day_of_month를 만든다.
    time_t t = time(NULL);
    struct tm first_day_of_month = *localtime(&t);

    first_day_of_month.tm_mday = 1;
    first_day_of_month.tm_mon = month;
    first_day_of_month.tm_year = year - 1900;
    mktime(&first_day_of_month);

    //first_day_of_month의 요일 정보를 저장.
    info.start_day = first_day_of_month.tm_wday;

    return info;
}

void show_calendar(int fd){ 

    //TODO: 서버에 유저의 아이디를 전송하여 출석 정보 저장및 현재 client의 출석기록 받아옴
    //TODO: 날짜 정보 처리 부분은 서버로 옮기는게 좋은지 논의
    
    char message_to_server[2048]; //서버에 보낼 메시지
    char message_form_server[2048]; //서버로부터 받은 메시지
    
    int arr[32] = {0,};
    int attended_day;
    int current_day=1;

    int row = 2; //0,1은 헤더가 사용중이기 때문
    int col;

    //오늘 날짜를 불러와서 time_t에 저장
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    //오늘의 日을 1로 함
    //TODO: 이 정보 서버와 통신하여 입/출력해야 함
    clear();
    curs_set(0); //커서를 안보이게 함

    //색상 팔레트 만들기: https://m.blog.naver.com/guile21c/53287736
    init_pair(1, 0, 7);
    // init_pair(2, COLOR_BLACK,COLOR_WHITE);

    //헤더- 달 출력
    move(0,(max_x/2)-4);
    addstr(MON[tm.tm_mon]);

    //헤더-요일 출력
    move(1,(max_x/2)-12);
    addstr("Su Mo Tu We Th Fr Sa");

    struct calendarInfo info=get_calender_info(tm);

    /*****************달력 그리기*******************/

    send(fd,message_to_server,2048,0);
    recv(fd, message_form_server,2048,0);

    // attendance: 다음 숫자 위치 찾기
    char* start = strstr(message_form_server, "attendance:");
    // attendance: 다음 숫자 위치로 이동
    start += strlen("attendance:");
    // 문자열 파싱하여 arr 배열에 숫자 저장
    int index = 0;
    char* token = strtok(start, ",");
    while (token != NULL && index < sizeof(arr) / sizeof(arr[0])) {
        arr[index++] = atoi(token);
        token = strtok(NULL, ",");
    }

    while (current_day <= info.number_of_days_in_month) {//current_day(지금 출력하고 있는 day)가 이번 달의 마지막 날일때 까지
        for (col = info.start_day ;  col < 7 && current_day <= info.number_of_days_in_month; col++) { 
            //시작 요일부터 6(일요일)까지 출력 후 줄바꿈하고 그 뒤로는 한 행에 7일씩 출력. 
            char day_str[3];
            snprintf(day_str, sizeof(day_str), "%2d", current_day); //day를 형식에 맞게 저장
            move(row, ((max_x/2)-12)+(col * 3));
            if (arr[current_day] == 1) { //arr[日]이 1인 경우는 색상을 반전시켜 출력          
                //출석o 목표달성x      
                attron(COLOR_PAIR(1));
                addstr(day_str);
                attroff(COLOR_PAIR(1));
                //목표o

            } else {
                addstr(day_str);
            }
            current_day++;
        }
        info.start_day = 0;
        row++;
    }
   
    move(menu_bar, (max_x-36)/2);
    printw("Press any key to close the calendar");
    refresh();
    nodelay(stdscr, FALSE);
    while (!getch());
    return;    
}