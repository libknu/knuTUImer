#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdbool.h>

struct calendarInfo
{
    int number_of_days_in_month;//이번 달은 며칠짜리?
    int start_day;//이번 달은 무슨 요일부터?
};

//TODO: 실제 구조체에는 더 많은 정보 필요(출석 정보 등...)
typedef struct test_member
{   
    char user_name[20];//유저 이름
    double study_time; //유저의 총 공부 시간
    char* joined_groups;//가입된 그룹
}tm;

extern tm dummy_members[20];

typedef struct test_group
{   
    char group_name[20];//그룹명
    double study_time; //그룹원의 총 공부 시간
    char *group_members; //그룹원 리스트
}tg;

extern tg dummy_groups[10];

typedef struct _time
{
    int hours;
    int minutes;
    int seconds;
}TIME;

static const char MON[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#define menu_bar_display move(menu_bar, (max_x-76)/2); \
printw("Home: 'H' | Start studying: 'S' | Manage groups: 'G' | Check attendance 'A'");
#define menu_bar_message move(menu_bar, (max_x-50)/2); \
printw("!!Menu bar is not available on the current page!!");


//TODO: 실제 구현 및 제출 시 삭제
void make_dummy_data();

//common.c
char* display_time(int hour, int minute, int second);
void get_terminal_size();
TIME convert_time_unit(double sec);

//calendar.c
struct calendarInfo get_calender_info(struct tm);
void show_calendar();

//first_page.c
void first_page();

//auth.c
void login();
void creatID();

//mypage.c
void mypage();

//timer_utils.c
void start_study();
void stopwatch_mode();
void timer_mode();
void pomodoro_mode();

//group_utils.c
void manage_groups();
void my_group_list();
void group_member_list();
void join_other_Group();
void make_my_Group();

//setup.c
int setup(int, char **);

#endif
