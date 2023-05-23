#include "client.h"
#include <stdlib.h>
#include <curses.h>

extern max_x, max_y;

void login(){
    
    //TODO :아이디와 비밀번호 처리하기
    char testID[20]="id"; //테스트용 아이디와 비밀번호.
    char testPassword[20]="pass"; //실제로는 파일에서 읽어와서 서치해야 함

    char id[20]; //id 입력값
    char password[20]; //password 입력값

    char ch;
    int i;

    /***************화면 구성***************/
    initscr();
    noecho();
    erase();// 화면 내용을 다 지움
    
    move(2, (max_x/2)-4); addstr("Log in");

    move(4, (max_x/2)-2);     addstr("ID: ");
    move(5, (max_x/2)-5);     addstr("Password: ");
    move(4, (max_x/2)+1); //커서를 ID에  맞게 이동
    refresh();

    //https://stackoverflow.com/questions/50014137/printw-plus-scanf-in-ncurse-window-wont-display-typed-text
    curs_set(1);
    nodelay(stdscr, FALSE);
    echo();
    scanw("%s", id);
    noecho();
    refresh();

    //비밀번호에 맞게 이동
    move(5, (max_x/2)+4);
    refresh();
    echo();
    scanw("%s", password);
    noecho();
    refresh();
    endwin();

    if((strcmp(id,testID)!=0)||(strcmp(password,testPassword)!=0)){//만약 아이디나 패스워드가 일치하지 않으면
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x/2)-17);
        addstr("The ID or password doesn't match.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        first_page();
    }else{//로그인 성공
        mypage();
    }
}

void creatID(){

    //TODO: ID 중복확인
    char testID[20]="id"; //테스트용 아이디. 실제로는 파일에서 읽어와서 서치해야 함
    
    char id[20]; //id 입력값
    char password[20]; //password 입력값

    char ch;
    int i;

    initscr();
    noecho();

    erase();// 화면 내용을 다 지움

    move(2, (max_x/2)-4);     addstr("Sign up");
    move(4, (max_x/2)-2);     addstr("ID: ");
    move(5, (max_x/2)-5);     addstr("Password: ");
    move(4, (max_x/2)+1); //커서를 ID에  맞게 이동
    refresh();
    curs_set(1);//일반 커서를 보여줌

    nodelay(stdscr, FALSE);
    echo();
    scanw("%s", id);
    noecho();
    refresh();
    endwin();


    //비밀번호에 맞게 이동
    move(5, (max_x/2)+4);
    refresh();
    echo();
    scanw("%s", password);
    noecho();
    refresh();
    endwin();

    if((strcmp(id,testID)==0)){//만약 아이디가 중복이면
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x/2)-13);  addstr("This ID is already exist.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        first_page();//아이디 만들기 다시 시작
    }else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x-42)/2);  addstr("A new ID has been created. Please log in.");
        refresh();
        sleep(1);
        endwin();
        first_page();
    }
}