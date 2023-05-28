#include "client.h"
#include <math.h>

extern int max_x, max_y, menu_bar;
int goal;

void set_goal(int fd){
    noecho();
    curs_set(0);
    erase();
    
    move(3, (max_x-33)/2);     addstr("Please Enter Today's Goal(hours)");
    refresh();

    goal = getch();

    move(5, (max_x-26)/2);     printw("Today's goal is %c hours!", goal);
    refresh();
    sleep(1);
}

void mypage(int fd){

    //TODO: 디버깅용으로 1로 설정하였으며, 실제로는 그 날의 첫 접속인지 확인해야 함
    //TODO: 서버에 유저 아이디를 전송하면, 서버는 오늘 날짜를 확인하여 해단 유저의 첫 접속인 경우, 아닌 경우를 나누어 리턴
    int First_access_flag=1;
    int command;
    //
    double focusing;
    TIME focusing_converted;

    if(First_access_flag==1){//하루 중 첫 접속이면
        show_calendar(fd);
        set_goal(fd);
    }

    /***************화면 구성***************/
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움

    move(1, (max_x/2)-13);     addstr("Today's Focusing Goal(hours)");
    move(2, (max_x/2)-4);     addstr(display_time(goal - 48, 0, 0));
    move(4, (max_x/2)-11);     addstr("Today's Total Focusing");
    
    focusing=600;
    focusing_converted=convert_time_unit(focusing);
    move(5, (max_x/2)-4);     addstr(display_time(focusing_converted.hours,focusing_converted.minutes,focusing_converted.seconds));
    refresh();  
    /****************************************/

    menu_bar_display
    /***************커맨드 입력***************/

    command = getch();
    if(command=='H'||command=='h'){
        mypage(fd);
    }else if(command=='S'||command=='s'){        
        start_study(fd);
    }else if(command=='G'||command=='g'){
        manage_groups(fd);
    }else if(command=='A'||command=='a'){
        show_calendar(fd);
        mypage(fd);
    }else{
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x/2)-15);
        addstr("Please enter the correct key.");
        menu_bar_message
        refresh();
        sleep(1);//1초간 오류를 보여주고
        mypage(fd);
    } 
}