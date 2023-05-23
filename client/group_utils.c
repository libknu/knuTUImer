#include "client.h"
#include<string.h>
#include<curses.h>

extern int menu_bar, max_x, max_y;

//TODO: 실제로는 사용하지 않음. ui확인용. 제출 시 삭제
extern tg dummy_groups[10];
extern tm dummy_members[20];

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

    //TODO: 현재 client의 ID를 서버에 보내고, 그 클라이언트가 가입한 그룹의 리스트를 서버로부터 받아야 함    
    int command;
    char enter_group_name[20];
    int list_maximum_num; //현재 터미널에서 표시 가능한 최대 그룹 개수
    int total_group_num; //클라이언트가 가입한 그룹의 개수
    int overflow_num; //화면에 표시되지 않고 스크롤링해야 보이는 그룹의 개수
    int scroll_num=0; //아래 방향키로 스크롤한 횟수
    initscr();
    noecho();
    erase();
    // raw(); 
    keypad(stdscr, TRUE);

    menu_bar_display
    move(2, (max_x-14)/2);
    printw("My Group List");

    list_maximum_num=max_y-7;
    total_group_num=sizeof(dummy_groups)/sizeof(dummy_groups[1]);
    overflow_num=total_group_num-list_maximum_num;

    move(1,1);
    printw("debug:%d", list_maximum_num);

    //TODO: 실제로는 array형태의 더미 데이터가 아니라 링크드 리스트 형태의 데이터를 서버로부터 받아 옴
    //TODO: 링크드리스트가 아니라 array형태로 받아오는거라면, 그대로 사용할 수 있음

    for(int i=0; i<total_group_num&&i<list_maximum_num;i++){
        move(3+i, (max_x- strlen(dummy_groups[i].group_name))/2);
        printw("%s",dummy_groups[i].group_name);
    }

    move(menu_bar-1,(max_x-42)/2);
    addstr("If you want to enter the group, press \"E\"");
    move(5, 47);

    // command = getchar();
    // if(!strcmp(command, "A")){
    //     // beep();
    // }

    while (1)
    {
        command = getch();
    
        //그룹에 입장하기
        if(command=='E'||command=='e'){        
            move(menu_bar-1,(max_x-42)/2); 
            addstr("                                             ");

            move(menu_bar,(max_x-76)/2); 
            addstr("                                                                                      ");
            menu_bar_message;
            move(menu_bar-1,(max_x/2)-20);
            addstr("Group name: ");
            move(menu_bar-1,(max_x/2)-8);
            curs_set(1);
            echo();
            scanw("%s",enter_group_name);
            noecho();

            curs_set(0);
            //TODO: 내가 속한 그룹 전체를 순회하며 내가 입력한 그룹이 존재하는지 확인
            //TODO: 혹은, 서버에 클라이언트의 ID와 입력한 그룹 명을 전송하면, 서버가 해당 클라이언트가 가입한 그룹 중 입력된 그룹이 있는지 확인해줌

            //존재하면 입장
            //존재하지 않으면 my_group_list() 재귀호출
            //존재한다고 가정하고 UI구현함
            group_member_list();

        }else if(command==KEY_UP){//위 방향키
            if(scroll_num!=0) scroll_num--;
            move(2,1);
            printw("%d",scroll_num);
        }else if(command==KEY_DOWN){//아래 방향키
            if(scroll_num!=total_group_num-list_maximum_num) scroll_num++;
            move(2,1);
            printw("%d",scroll_num);
        }else if(command=='H'||command=='h'){
            mypage();
        }else if(command=='S'||command=='s'){        
            start_study();
        }else if(command=='G'||command=='g'){
            manage_groups();
        }else if(command=='A'||command=='a'){
            show_calendar();
            my_group_list();
        }
        else{
            initscr();
            erase();// 화면 내용을 다 지운 뒤
            move(2, (max_x-30)/2);
            printw("Please enter the correct key.[%d][%c]",command,command);
            refresh();
            sleep(1);//1초간 오류를 보여주고
            endwin();
            my_group_list();
        }

        for(int i=scroll_num; i<total_group_num&&i<list_maximum_num+scroll_num;i++){
            move(3+i-scroll_num,(max_x- strlen(dummy_groups[i].group_name)));
            addstr("                                                ");
            move(3+i-scroll_num, (max_x- strlen(dummy_groups[i].group_name))/2);
            printw("%s",dummy_groups[i].group_name);
        }
        refresh();
    }
}

void group_member_list(){

    //TODO: 실제로는 그룹명을 서버로 전송하여 해당 그룹의 멤버 리스트를 받아와야 함
    int command;
    char member_to_call[20];
    int list_maximum_num;
    int total_member_num; //해당 그룹의 멤버 수
    int overflow_num; //화면에 표시되지 않고 스크롤링해야 보이는 멤버 수
    int scroll_num=0; //아래 방향키로 스크롤한 횟수

    list_maximum_num=max_y-8;
    total_member_num=sizeof(dummy_members)/sizeof(dummy_members[1]);
    overflow_num=total_member_num-list_maximum_num;

    initscr();
    noecho();
    erase();
    menu_bar_display


    //TODO: 실제로는 array형태의 더미 데이터가 아니라 링크드 리스트 형태의 데이터를 서버로부터 받아 옴
    //TODO: 링크드리스트가 아니라 array형태로 받아오는거라면, 그대로 사용할 수 있음
    move(2, (max_x-11)/2);
    printw("Member List");

    move(1,1);
    printw("debug:%d", list_maximum_num);

    //TODO: 실제로는 array형태의 더미 데이터가 아니라 링크드 리스트 형태의 데이터를 서버로부터 받아 옴

    for(int i=0; i<total_member_num&&i<list_maximum_num;i++){
        move(3+i, (max_x- strlen(dummy_members[i].user_name))/2);
        printw("%s",dummy_members[i].user_name);
    }

    move(menu_bar-2,(max_x-39)/2);
    addstr("If you want to call someone, press \"W\"");
    
    move(menu_bar-1,(max_x-31)/2);
    addstr("If you want to chat, press \"C\"");
    move(5, 47);


    while (1)
    {
         command = getch();

    //TODO: 멤버 깨우기(미구현)
    //TODO: 서버에 client 의 ID와 깨울 멤버의 ID(즉, 이름)을 보내면 서버가 그것을 깨울 멤버에게 전송
    //TODO: 근데 깨움을 accept하는 부분은 아직 미구현...이고 미구상임
        if(command=='W'||command=='w'){        
            move(menu_bar-1,(max_x-42)/2); 
            addstr("                                             ");

            move(menu_bar,(max_x-76)/2); 
            addstr("                                                                                      ");
            menu_bar_message;
            move(menu_bar-1,(max_x/2)-20);
            addstr("Member name: ");
            move(menu_bar-1,(max_x/2)-8);
            curs_set(1);
            echo();
            scanw("%s",member_to_call);
            noecho();

            curs_set(0);
            //TODO: 서버에 현재 클라이언트의 아이디와 현재 그룹명, 깨울 멤버 아이디를 전송
            //현재 그룹에 깨울 멤버가 존재하면 그 멤버에게 깨움 전송...
            //존재하지 않으면 group_member_list() 재귀호출
            //존재하지 않는다고 가정하고 UI구현함
            group_member_list();
        }
        //채팅창으로 입장하기
        else if(command=='C'||command=='c'){
            //TODO: 채팅 구현
            group_member_list();
        }else if(command==KEY_UP){//위 방향키
            if(scroll_num!=0) scroll_num--;
            move(2,1);
            printw("%d",scroll_num);
        }else if(command==KEY_DOWN){//아래 방향키
            if(scroll_num!=total_member_num-list_maximum_num) scroll_num++;
            move(2,1);
            printw("%d",scroll_num);
        }
        else if(command=='H'||command=='h'){
            mypage();
        }else if(command=='S'||command=='s'){        
            start_study();
        }else if(command=='G'||command=='g'){
            manage_groups();
        }else if(command=='A'||command=='a'){
            show_calendar();
            group_member_list();
        }
        else{
            initscr();
            erase();// 화면 내용을 다 지운 뒤
            move(2, (max_x-30)/2);
            addstr("Please enter the correct key.");
            refresh();
            sleep(1);//1초간 오류를 보여주고
            endwin();
            group_member_list();
        }

        for(int i=scroll_num; i<total_member_num&&i<list_maximum_num+scroll_num;i++){
            move(3+i-scroll_num,(max_x- strlen(dummy_members[i].user_name)));
            addstr("                                                ");
            move(3+i-scroll_num, (max_x- strlen(dummy_members[i].user_name))/2);
            printw("%s",dummy_members[i].user_name);
        }
        refresh();

    }
}

void join_other_Group(){

    //TODO: 서버에 현재 client의 ID와 가입하려는 그룹의 이름을 전송하면, join성공 여부 반환. 실패라면, 존재하지 않는 그룹인 경우와 이미 가입된 그룹인 경우로 나누어 리턴값 받기

    char group_for_join[20];
    
    initscr();
    erase();

    move(2, (max_x-14)/2);
    printw("Joining Group");
    move(menu_bar,(max_x-76)/2); 
    addstr("                                                                                      ");
    menu_bar_message;
    move(menu_bar-1, (max_x-54)/2);
    printw("If you want to cancel the group join, type \"B\" or \"b\"");

    move(3,(max_x/2)-20);
    addstr("Group name: ");
    move(3,(max_x/2)-8);
    curs_set(1);
    echo();
    scanw("%s",group_for_join);
    noecho();


    //b가 입력됐을 때(이전 페이지인 manage_groups()호출)
    //이미 가입된 그룹일 때 -> 오류(오류 메시지를 띄운 뒤 이전 페이지인 manage_groups()호출)
    //존재하지 않는 그룹일 때 -> 오류(오류 메시지를 띄운 뒤 이전 페이지인 manage_groups()호출)
    //가입
    if(strcmp("B",group_for_join)==0||strcmp("b",group_for_join)){
        manage_groups();
    }else{
        manage_groups();
    }

    refresh();
    endwin();
}

void make_my_Group(){

    //TODO: 서버에 현재 client의 ID와 만드려는 그룹의 이름을 전송하면, 서버는 이미 존재하는 그룹인지 확인하고, make성공 여부 반환.

    char group_for_make[20];
    
    initscr();
    erase();

    move(2, (max_x-14)/2);
    printw("Making Group");
    move(menu_bar,(max_x-76)/2); 
    addstr("                                                                                      ");
    menu_bar_message;
    move(menu_bar-1, (max_x-56)/2);
    printw("If you want to cancel the group making, type \"B\" or \"b\"");

    move(3,(max_x/2)-20);
    addstr("Group name: ");
    move(3,(max_x/2)-8);
    curs_set(1);
    echo();
    scanw("%s",group_for_make);
    noecho();

    //b가 입력됐을 때(이전 페이지인 manage_groups()호출)
    //이미 존재하는 그룹일 때 -> 오류(오류 메시지를 띄운 뒤 이전 페이지인 manage_groups()호출)
    //완성
    if(strcmp("B",group_for_make)==0||strcmp("b",group_for_make)){
        manage_groups();
    }else{
        manage_groups();
    }

    refresh();
    endwin();
}
