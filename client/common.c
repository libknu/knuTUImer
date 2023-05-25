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