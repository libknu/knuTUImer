#include "client.h"
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

extern int max_x, max_y;

void login(int fd){    
    //TODO : 서버에 아이디를 전송하면 서버가 로그인 성공 여부 반환

    char id[20]; //id 입력값
    char password[20]; //password 입력값
    char message_to_server[2048]; //서버에 보낼 메시지
    char message_from_server[2048]; //서버로부터 받은 메시지

    /***************화면 구성***************/
    noecho();
    erase();// 화면 내용을 다 지움
    move(2, (max_x/2)-4);     addstr("Log in");
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

    sprintf(message_to_server,"login:%s,%s",id,password);
    send(fd,message_to_server,2048,0);
    recv(fd, message_from_server,2048,0);
    
    //move(1,1);
    //printw("%s",message_from_server);
    refresh();
    if(strcmp(message_from_server,"FAILED")==0){//만약 아이디나 패스워드가 일치하지 않으면
        erase();// 화면 내용을 다 지운 뒤
        curs_set(0);
        move(2, (max_x/2)-17);
        addstr("The ID or password doesn't match.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        first_page(fd);
    }else{//로그인 성공
        mypage(fd);
    }
}

void creatID(int fd){

    //TODO : 서버에 아이디를 전송하면 서버가 회원가입 성공 여부 반환
    
    char id[20]; //id 입력값
    char password[20]; //password 입력값
    char message_to_server[2048]; //서버에 보낼 메시지
    char message_from_server[2048]; //서버로부터 받은 메시지

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

    sprintf(message_to_server,"signin:%s,%s",id,password);
    send(fd, message_to_server, 2048,0);

    recv(fd, message_from_server,2048,0);
    // printw("%s",message_from_server);

    if(strcmp(message_from_server,"FAILED")==0){//만약 아이디가 중복이면
        erase();// 화면 내용을 다 지운 뒤
        curs_set(0);
        move(2, (max_x/2)-13);  addstr("This ID is already exist.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        first_page(fd);//아이디 만들기 다시 시작
    }else if(strcmp(message_from_server,"SUCCESS")==0){
        erase();// 화면 내용을 다 지운 뒤
        curs_set(0);
        move(2, (max_x-42)/2);  addstr("A new ID has been created. Please log in.");
        refresh();
        sleep(1);
        first_page(fd);
    }
}