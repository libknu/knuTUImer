#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdbool.h>

struct calendarInfo
{
    int number_of_days_in_month;//이번 달은 며칠짜리?
    int start_day;//이번 달은 무슨 요일부터?
};

const static char MON[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

//main.c
void first_page();//첫 시작 페이지. 로그인 또는 회원가입
void login();//로그인
void creatID();//회원가입
void mypage();//show_calendar(하루 중 1회만. 4초간 보여줌.)/Today’s Total Focusing(보여만 줌)/ 공부 시작하기(stdin(1)) / 그룹 관리하기(stdin(2))-goole docs에서 바뀐 부분
struct calendarInfo get_calender_info(struct tm);
void show_calendar(); //하루 중 첫 접속때만 first page에서 호출
char* display_time(int hour, int minute, int second);
void start_study(); //Choose mode(스톱워치(stdin(1)), 타이머(stdin(2)), 뽀모도로(stdin(3)))
void stopwatch_mode();
void timer_mode();
void pomodoro_mode();
void manage_groups(); //Group List(stdin(1)) Join other Group(stdin(2)) Make my Group(stdin(3))
void group_list();
bool is_exist_member(char[]);
void join_other_Group();
void make_my_Group();
void get_terminal_width();//화면 중앙에 오브젝트를 배치하기 위해 터미널의 가로 길이를 구하는 함수이다


//setup.c
int setup(int, char **);

#endif
