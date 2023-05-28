#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdbool.h>

struct calendarInfo
{
    int number_of_days_in_month;//이번 달은 며칠짜리?
    int start_day;//이번 달은 무슨 요일부터?
};

typedef struct _time
{
    int hours;
    int minutes;
    int seconds;
} TIME;

static const char MON[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#define menu_bar_display move(menu_bar, (max_x-76)/2); \
printw("Home: 'H' | Start studying: 'S' | Manage groups: 'G' | Check attendance 'A'");
#define menu_bar_message move(menu_bar, (max_x-50)/2); \
printw("!!Menu bar is not available on the current page!!");


//common.c
char* display_time(int hour, int minute, int second);
void get_terminal_size();
TIME convert_time_unit(double sec);
bool is_exist(char* input, char arr[][10],int len);

//calendar.c
struct calendarInfo get_calender_info(struct tm);
void show_calendar(int fd);

//first_page.c
void first_page(int fd);

//auth.c
void login(int fd);
void creatID(int fd);

//mypage.c
void mypage(int fd);

//timer_utils.c
void start_study(int fd);
void stopwatch_mode(int fd);
void timer_mode(int fd);
void pomodoro_mode(int fd);

//group_utils.c
void manage_groups(int fd);
void my_group_list(int fd);
void group_member_list(int fd, char* group);
void join_other_Group(int fd);
void make_my_Group(int fd);


//chat.c
void group_chat(int fd);

//setup.c
int setup(int, char **);

#endif
