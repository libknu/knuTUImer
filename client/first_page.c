#include "client.h"

extern int max_x, max_y;

void first_page(){
    //첫 접속 시 나타나는 창이다. 로그인을 하거나, 회원가입을 한다.
    int command;
    /***************화면 구성***************/
    initscr();
    noecho();
    start_color();
    erase();// 화면 내용을 다 지움

    move(2, (max_x/2)-12);    addstr("log in-Please enter '1'");
    move(3, (max_x/2)-12);    addstr("sign up-Please enter '2'");
    curs_set(0);//커서를 안보이게 설정

    refresh();
    /****************************************/

    /***************커맨드 입력***************/
    command = getch();
    if(command=='1'){        
        login();
    }else if(command=='2'){
        creatID();
    }else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x/2)-15);     addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        first_page();
    }
    /****************************************/
    endwin();
}