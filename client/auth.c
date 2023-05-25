#include "client.h"
#include <stdlib.h>
#include <curses.h>

extern max_x, max_y;

void login(){    
    //TODO : 서버에 아이디를 전송하면 서버가 로그인 성공 여부 반환

    char testID[20]="id"; //테스트용 아이디와 비밀번호.
    char testPassword[20]="pass"; //실제로는 서버단에서 확인


    char id[20]; //id 입력값
    char password[20]; //password 입력값


    /***************화면 구성***************/
    noecho();
    erase();// 화면 내용을 다 지움
    move(2, (max_x/2)-4); addstr("Log in");
    move(4, (max_x/2)-2);     addstr("ID: ");
    move(5, (max_x/2)-5);     addstr("Password: ");
    move(4, (max_x/2)+1); //커서를 ID에  맞게 이동

    //https://stackoverflow.com/questions/50014137/printw-plus-scanf-in-ncurse-window-wont-display-typed-text
    curs_set(1);
    echo();
    scanw("%s", id);

    //비밀번호에 맞게 이동
    move(5, (max_x/2)+4);
    scanw("%s", password);
    noecho();

    //TODO: 조건문을 수정. 서버가 로그인 성공을 반환하면
    if((strcmp(id,testID)!=0)||(strcmp(password,testPassword)!=0)){//만약 아이디나 패스워드가 일치하지 않으면
        erase();// 화면 내용을 다 지운 뒤
        curs_set(0);
        move(2, (max_x/2)-17);
        addstr("The ID or password doesn't match.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        first_page();
    }else{//로그인 성공
        mypage();
    }
}

void creatID(){

    //TODO : 서버에 아이디를 전송하면 서버가 회원가입 성공 여부 반환

    char testID[20]="id"; //테스트용 아이디. 실제로는 파일에서 읽어와서 서치해야 함
    
    char id[20]; //id 입력값
    char password[20]; //password 입력값

    // initscr();
    // noecho();

    erase();// 화면 내용을 다 지움

    move(2, (max_x/2)-4);     addstr("Sign up");
    move(4, (max_x/2)-2);     addstr("ID: ");
    move(5, (max_x/2)-5);     addstr("Password: ");
    move(4, (max_x/2)+1); //커서를 ID에  맞게 이동
    refresh();
    curs_set(1);//일반 커서를 보여줌

    echo();
    scanw("%s", id);

    //비밀번호에 맞게 이동
    move(5, (max_x/2)+4);
    scanw("%s", password);

    //TODO: 서버로부터 회원가입 성공 여부를 받아오도록 수정
    if((strcmp(id,testID)==0)){//만약 아이디가 중복이면
        erase();// 화면 내용을 다 지운 뒤
        curs_set(0);
        move(2, (max_x/2)-13);  addstr("This ID is already exist.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        first_page();//아이디 만들기 다시 시작
    }else{
        erase();// 화면 내용을 다 지운 뒤
        curs_set(0);
        move(2, (max_x-42)/2);  addstr("A new ID has been created. Please log in.");
        refresh();
        sleep(1);
        first_page();
    }
}