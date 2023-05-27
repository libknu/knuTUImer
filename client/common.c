#include<curses.h>
#include "client.h"

int max_x, max_y;
int menu_bar;

char* display_time(int hour, int minute, int second){
	static char str[50];
    sprintf(str,"%02d:%02d:%02d", hour, minute, second);
	return str;
}

void get_terminal_size(){
    initscr();
    getmaxyx(stdscr,max_y,max_x);
    menu_bar=max_y-3;
}

TIME convert_time_unit(double sec){
    TIME new_time;
    new_time.hours = sec / 3600;
    new_time.minutes = ((int)sec % 3600) / 60;
    new_time.seconds = (int)sec % 60;
    return new_time;
}

bool is_exist(char* input, char arr[][10], int len){
    for (int i = 0; i < len; i++) {
        if (strcmp(input, arr[i]) == 0) {
            return true;
        }
    }
    return false;
}
