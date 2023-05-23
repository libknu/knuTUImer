#include "client.h"

extern int menu_bar, max_x, max_y;

void manage_groups(){
    int command;
    
    /***************화면 구성***************/
    initscr();
    curs_set(0);
    clear();
    menu_bar_display
    move(2,(max_x-19)/2);
    addstr("Choose an option:\n");
    move(3,(max_x-15)/2);
    addstr("1. Group List\n");
    move(4,(max_x-21)/2);
    addstr("2. Join other Group\n");
    move(5,(max_x-18)/2);
    addstr("3. Make my Group\n");
    refresh();
    /****************************************/

    /****************커맨드 입력****************/
    command = getch();
    if(command=='1'){        
        my_group_list();
    }else if(command=='2'){
        join_other_Group();
    }else if(command=='3'){
        make_my_Group();
    }else if(command=='H'||command=='h'){
        mypage();
    }else if(command=='S'||command=='s'){        
        start_study();
    }else if(command=='G'||command=='g'){
        manage_groups();
    }else if(command=='A'||command=='a'){
        show_calendar();
        manage_groups();
    }
    else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x-30)/2);
        addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        manage_groups();
    }
    /****************************************/
}
void my_group_list(){

    int command;
    char member_to_call[20];
    initscr();
    noecho();
    erase();
    // menu_bar_display

    move(2, (max_x-18)/2);
    printw("[%s]My Group List", "Juwon");
    move(3, (max_x-7)/2);
    printw("group1"); 
    move(4,(max_x-7)/2);
    printw("group2"); 
    refresh();

    move(menu_bar-1,(max_x-31)/2);
    addstr("If you want to chat, press \"C\"");
    move(5, 47);

    // command = getchar();
    // if(!strcmp(command, "A")){
    //     // beep();
    // }

    if(getch()) group_member_list();

    refresh();
    sleep(1);
    endwin();
}

void group_member_list(){
    //TODO: group 은 구현이 끝나면 메뉴바를 만들겠음
    //여기는 나중에 반복문으로 유저 목록을 출력해야 하는데, 현재상태로는 어려워서 단순출력함.
    //구조체 멤버가 정해지면 반복문 형태로 수정
    int command;
    char member_to_call[20];
    initscr();
    noecho();
    erase();
    // menu_bar_display

    move(2, (max_x-11)/2);
    printw("[%s]member List", "group1");
    move(3, (max_x-35)/2);
    printw("%s's total focusing time %02d:%02d","member1",5, 20); 
    move(4,(max_x-35)/2);
    printw("%s's total focusing time %02d:%02d","member2",5, 20); 
    refresh();

    move(menu_bar-2,(max_x-39)/2);
    addstr("If you want to call someone, press \"A\"");
    
    move(menu_bar-1,(max_x-31)/2);
    addstr("If you want to chat, press \"C\"");
    move(5, 47);

    // command = getchar();
    // if(!strcmp(command, "A")){
    //     // beep();
    // }

    refresh();
    sleep(1);
    endwin();
    group_member_list();
}

void join_other_Group(){
    initscr();
    erase();

    //그룹명을 입력
    //이미 가입된 그룹일 때 -> 오류
    //존재하지 않는 그룹일 때 -> 오류
    //가입
    refresh();
    endwin();
}


void make_my_Group(){
    initscr();
    erase();
    refresh();
    endwin();
}
