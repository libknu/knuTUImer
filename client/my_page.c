#include "client.h"

extern int max_x, max_y, menu_bar;
int goal;

void set_goal(){
    initscr();
    noecho();
    curs_set(0);
    erase();
    
    move(3, (max_x/2)-18);     addstr("Please Enter Today's Goal(hours)");
    refresh();

    goal = getch();

    move(5, (max_x/2)-18);     printw("Today's goal is %c hours!", goal);
    refresh();
    sleep(1);

}

void mypage(){
    int First_access_flag=1;//TODO: 디버깅용으로 1로 설정하였으며, 실제로는 그 날의 첫 접속인지 확인해야 함
    int command;

    if(First_access_flag==1){//하루 중 첫 접속이면
        show_calendar();
        set_goal();

    }

    /***************화면 구성***************/
    initscr();
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움

    move(1, (max_x/2)-13);     addstr("Today's Focusing Goal(hours)");
    move(2, (max_x/2)-4);     addstr(display_time(goal - 48, 0, 0));
    move(4, (max_x/2)-11);     addstr("Today's Total Focusing");
    
    move(5, (max_x/2)-4);     addstr(display_time(5,0,0));//TODO: 실제로는 총 공부 시간을 파일에서 읽어오는 함수를 새로 만들어야 함.    
    refresh();  
    /****************************************/

    menu_bar_display
    /***************커맨드 입력***************/

    command = getch();
    if(command=='H'||command=='h'){
        mypage();
    }else if(command=='S'||command=='s'){        
        start_study();
    }else if(command=='G'||command=='g'){
        manage_groups();
    }else if(command=='A'||command=='a'){
        show_calendar();
        mypage();
    }else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x/2)-15);
        addstr("Please enter the correct key.");
        menu_bar_message
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        mypage();
    }  
    endwin();
}
