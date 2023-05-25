#include "client.h"
#include <time.h>
#include <curses.h>

extern int max_x, max_y, menu_bar;

void start_study(){ //Choose mode(스톱워치(stdin(1)), 타이머(stdin(2)), 뽀모도로(stdin(3)))
    int command;
    
    /***************화면 구성***************/
    curs_set(0);
    clear();
    move(2,(max_x-19)/2);      addstr("Choose an option:\n");
    move(3,(max_x-10)/2);      addstr("1. Timer\n");
    move(4,(max_x-14)/2);      addstr("2. Stopwatch\n");
    move(5,(max_x-13)/2);      addstr("3. Pomodoro\n");
    menu_bar_display    
    /****************************************/

    /****************커맨드 입력****************/
    command = getch();
    if(command=='1'){        
        timer_mode();
    }else if(command=='2'){
        stopwatch_mode();
    }else if(command=='3'){
        pomodoro_mode();
    }else if(command=='H'||command=='h'){
        mypage();
    }else if(command=='S'||command=='s'){        
        start_study();
    }else if(command=='G'||command=='g'){
        manage_groups();
        start_study();
    }else if(command=='A'||command=='a'){
        show_calendar();
        start_study();
    }else{
        erase();// 화면 내용을 다 지운 뒤
        move(2, (max_x/2)-15);     addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        start_study();
    }
    /****************************************/
}

void stopwatch_mode(){ //time up

    //TODO: 유저의 아이디와 유저가 추가로 한 공부시간을 서버로 전송하면, 서버는 유저의 기존 공부시간에 추가 공부시간을 더해야 함 
    int command;
    bool running_flag = false;
    bool in_stopwatch_flag = true;
    bool start_flag=true;
    time_t start_time, current_time, pause_start_time, pause_end_time;
    double pause_time;//휴식 시간의 총합
    double elapsed_seconds;//현재시간-시작시간-휴식시간
    int hours, minutes, seconds;

    /***************화면 구성***************/
    noecho();

    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움
    move(2, (max_x-15)/2);     addstr("Stopwatch mode");
    move(4, (max_x-23)/2);     addstr("Start-Please enter '1'");
    move(5, (max_x-23)/2);     addstr("Pause-Please enter '2'");
    move(6, (max_x-24)/2);     addstr("Finish-Please enter '3'");
    /****************************************/
    menu_bar_display

    nodelay(stdscr, TRUE);
    
    if(start_flag == true){
        move(3,(max_x/2)-9);
        addstr(display_time(0,0,0));
    }
    while (in_stopwatch_flag){
        command = getch();
        // 각 조건에 적당한 처리를 해 주어야 함
        if (command != ERR){
            if (command == '1' && !running_flag){ // 작동중
                running_flag = true;
                if (start_flag == true){ // 첫 시작일때
                    time(&start_time);
                    start_flag = false;
                }
                else{ // 휴식 후 재개했을 때
                    time(&pause_end_time);
                    pause_time += difftime(pause_end_time, pause_start_time);
                }
                move(3, (max_x/2)+2);
                addstr("                     ");
                move(3, (max_x/2)+2);
                addstr("running...");
            }
            else if (command == '2'&& running_flag){ // 휴식 시작
                time(&pause_start_time);
                running_flag = false;
                move(3, (max_x/2)+2);
                addstr("                     ");
                move(3, (max_x/2)+2);
                addstr("pause!");
            }else if(command=='a'|| command=='A'){
                if(running_flag){
                    time(&pause_start_time);
                }
                show_calendar();
                running_flag = false;
                command='1';
                nodelay(stdscr, TRUE);
                //출결 기록을 띄우고 나면 화면을 다시 구성해야 함.
                erase();
                move(3,(max_x/2)-9);
                addstr(display_time(hours, minutes, seconds));
                move(3, (max_x/2)+2);   addstr("pause!");
                move(2, (max_x - 15) / 2);      addstr("Stopwatch mode");
                move(4, (max_x - 23) / 2);      addstr("Start-Please enter '1'");
                move(5, (max_x - 23) / 2);      addstr("Pause-Please enter '2'");
                move(6, (max_x - 24) / 2);      addstr("Finish-Please enter '3'");
                menu_bar_display
            }
            else if (command == '3'||command=='H'||command=='h'|| command=='S'||command=='s'||command=='G'||command=='g'){
                running_flag=false;
                in_stopwatch_flag = false;
            }
        }

        if(running_flag){
            move(3,(max_x/2)-9);
            time(&current_time);
            elapsed_seconds = difftime(current_time, start_time) - pause_time;
            hours = (int)elapsed_seconds / 3600;
            minutes = ((int)elapsed_seconds % 3600) / 60;
            seconds = (int)elapsed_seconds % 60;
            addstr(display_time(hours, minutes, seconds));
            move(7, 2);
            refresh();
        }
        
        usleep(100000);//0.1초마다 순회
    }
    
    nodelay(stdscr, FALSE);
    
    //TODO: 여기서 elapsed_seconds 유저 아이디와 함께 서버로 전송
    if(command == '3'||command=='s'||command=='S'){
        start_study();
    }else if(command=='H'||command=='h'){
        mypage();
    }else if(command=='G'||command=='g'){
        manage_groups();
    }
    endwin();
}

void timer_mode(){ //time down
    //TODO: 유저의 아이디와 유저가 추가로 한 공부시간을 서버로 전송하면, 서버는 유저의 기존 공부시간에 추가 공부시간을 더해야 함 
    int command;
    bool start_flag=true;
    int goal_hour, goal_minute;
    bool running_flag = false;
    bool in_timer_flag = true;
    
    time_t start_time, current_time, pause_start_time, pause_end_time;
    double pause_time;//휴식 시간의 총합
    double elapsed_seconds;//현재시간-시작시간-휴식시간
    double remaining_seconds; //목표시간-elapsed_seconds 해서 화면에 표시
    int hours, minutes, seconds;

    /***************화면 구성***************/
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움
    menu_bar_message
    move(2, (max_x-11)/2);     addstr("Timer mode");
    move(3, (max_x-18)/2);     addstr("Insert your goal!");
    move(4, (max_x-8)/2);     addstr("Hours: ");
    move(5, (max_x-10)/2);     addstr("Minutes: ");
    move(4, ((max_x-8)/2)+6); //커서를 hour 에 맞게 이동
    curs_set(1);//일반 커서를 보여줌

    nodelay(stdscr, FALSE);
    echo();
    scanw("%d", &goal_hour);
    nodelay(stdscr, TRUE);
    noecho();

    //minutes에 맞게 이동
    move(5, ((max_x-8)/2)+7);
    nodelay(stdscr, FALSE);
    echo();
    scanw("%d", &goal_minute);
    nodelay(stdscr, TRUE);
    noecho();

    erase();
    menu_bar_display
    curs_set(0);

    move(2, (max_x-11)/2);     addstr("Timer mode");
    move(3, ((max_x-15)/2)-10);     addstr("Remaining time");

    move(4, (max_x-23)/2);     addstr("Start-Please enter '1'");
    move(5, (max_x-23)/2);     addstr("Pause-Please enter '2'");
    move(6, (max_x-24)/2);     addstr("Finish-Please enter '3'");
    refresh();  
    /****************************************/

    if(start_flag == true){
        move(3,((max_x-15)/2)+6);
        addstr(display_time(goal_hour,goal_minute,0));
    }
    while (in_timer_flag) {
        command = getch();
        if (command != ERR){
            if (command == '1' && !running_flag){//작동중(시간이 줄어드는 중)
                running_flag = true;
                if (start_flag == true){ // 첫 시작일때
                    time(&start_time);
                    start_flag = false;
                }
                else{ // 휴식 후 재개했을 때
                    time(&pause_end_time);
                    pause_time += difftime(pause_end_time, pause_start_time);
                }
                move(3, ((max_x-15)/2)+16);
                addstr("                     ");
                move(3, ((max_x-15)/2)+16);
                addstr("running...");
            }
            else if (command == '2' && running_flag){//작동 중지
                time(&pause_start_time);
                running_flag = false;
                move(3, ((max_x-15)/2)+16);
                addstr("                     ");
                move(3, ((max_x-15)/2)+16);
                addstr("pause!");
            }else if(command=='a'|| command=='A'){
                if(running_flag){
                    time(&pause_start_time);
                }
                running_flag=false;
                show_calendar();
                nodelay(stdscr, TRUE);
                //출결 기록을 띄우고 나면 화면을 다시 구성해야 함.
                erase();
                move(3,((max_x-15)/2)+6);
                addstr(display_time(hours, minutes, seconds));
                move(3, ((max_x-15)/2)+16);    addstr("pause!");
                move(3, ((max_x-15)/2)-10);     addstr("Remaining time");
                move(2, (max_x - 11) / 2);      addstr("Timer mode");
                move(4, (max_x - 23) / 2);      addstr("Start-Please enter '1'");
                move(5, (max_x - 23) / 2);      addstr("Pause-Please enter '2'");
                move(6, (max_x - 24) / 2);      addstr("Finish-Please enter '3'");
                menu_bar_display
            }
            else if (command == '3'||command=='H'||command=='h'|| command=='S'||command=='s'||command=='G'||command=='g'){
                running_flag=false;
                in_timer_flag = false;
            }
        }
        if (running_flag){//작동중이면 계속 시간을 업데이트 해서 출력
            time(&current_time);
            elapsed_seconds = difftime(current_time, start_time) - pause_time;
            remaining_seconds= (goal_hour*3600)+(goal_minute*60) - elapsed_seconds;

            hours = (int)remaining_seconds / 3600;
            minutes = ((int)remaining_seconds % 3600) / 60;
            seconds = (int)remaining_seconds % 60;

            if (remaining_seconds <= 0) {
                remaining_seconds = 0;
                in_timer_flag = false;
            //     beep();
            }

            move(3,((max_x-15)/2)+6);
            addstr(display_time(hours, minutes, seconds));
            refresh();
        }
        usleep(100000);
    } 
    
    nodelay(stdscr, FALSE);
    
    //TODO: 여기서 elapsed_seconds 유저 아이디와 함께 서버로 전송
    if(command == '3'||command=='s'||command=='S'){
        start_study();
    }else if(command=='H'||command=='h'){
        mypage();
    }else if(command=='G'||command=='g'){
        manage_groups();
    }
}

void pomodoro_mode(){
    //TODO: 유저의 아이디와 유저가 추가로 한 공부시간을 서버로 전송하면, 서버는 유저의 기존 공부시간에 추가 공부시간을 더해야 함     
    int goal_sets;
    int command;

    bool running_flag = false; //포모도로 시간이 흐르는 중인지 확인하는 플래그
    bool in_pomo_flag = true;  //포모도로 타이머 자체가 작동중인지 확인하는 플래그. false면 함수가 끝남

    bool start_flag=true;

    time_t start_time, current_time, pause_start_time, pause_end_time;
    
    // time_t work_start_time, work_current_time, work_pause_start_time, work_pause_end_time;
    // time_t rest_start_time, rest_current_time, rest_pause_start_time, rest_pause_end_time;
    
    double pause_time, elapsed_seconds;

    double work_pause_time; //공부중일 때, 현재 세트에서의 일시중지 시간의 총합
    double work_elapsed_seconds; //공부중일 때, 현재 세트에서의 총 작동 시간. 현재시간-시작시간-일시중지시간
    double rest_pause_time; //쉬는시간일 때, 현재 세트에서의 일시중지 시간의 총합
    double rest_elapsed_seconds; //쉬는 시간일 때, 현재 세트에서의 총 작동 시간
    double total_elapsed_seconds=0; //총 공부시간(저장용)
    int hours, minutes, seconds;
    int set_counter = 0; //지금까지 몇 세트 했는지 저장

    int work_duration = 25*60; //공부는 25분간
    int rest_duration = 5*60; //휴식은 5분간 
    int remaining_seconds = work_duration;//쉬는 시간이든 공부 시간이든, 남은 시간
    int current_duration = work_duration;
    bool work_period_flag = true; //25분 사이클 안에 있는지 표시하는 플래그

    noecho();
    curs_set(0);
    erase();
    menu_bar_message
    move(2, (max_x-14)/2);     addstr("Pomodoro mode");
    move(3, (max_x-27)/2);     addstr("Enter the number of sets: ");
    curs_set(1);
    nodelay(stdscr, FALSE);
    echo();
    scanw("%d", &goal_sets);
    nodelay(stdscr, TRUE);
    
    noecho();
    curs_set(0);

    erase();
    menu_bar_display
    move(2, (max_x-14)/2);     addstr("Pomodoro mode");
    move(5, (max_x-23)/2);     addstr("Start-Please enter '1'");
    move(6, (max_x-23)/2);     addstr("Pause-Please enter '2'");
    move(7, (max_x-24)/2);     addstr("Finish-Please enter '3'");

    if(start_flag == true){ //처음 1을 입력하기 전에 표시할 내용
        move(3, (max_x-17)/2);
        printw("Work: Set %d/%d ",1, goal_sets);
        move(4, ((max_x-18)/2)-4);
        printw("remaining %02d:%02d",25, 0); 
    }
    while (in_pomo_flag && set_counter < goal_sets) {
        command = getch();
        if (command != ERR){
            if (command == '1' && !running_flag) {//작동중
                running_flag = true;
                if (start_flag == true){ // 첫 시작일때
                    time(&start_time);
                    start_flag = false;
                }
                else{ //일지중지 후 재개했을 때
                    if(work_period_flag)//일하는 중이면
                        time(&pause_end_time);
                        pause_time += difftime(pause_end_time, pause_start_time);
                }
                move(4, ((max_x-18)/2)+14);
                addstr("                ");
                move(4, ((max_x-18)/2)+14);
                addstr("running...");
                running_flag = true;
            } else if (command == '2' && running_flag) {//일시중지중. 공부중, 일하는 중 모두에서 작동
                time(&pause_start_time);
                running_flag = false;
                move(4, ((max_x-18)/2)+14);
                addstr("                ");
                move(4, ((max_x-18)/2)+14);
                addstr("pause!");
            }else if(command=='a'|| command=='A'){
                if(running_flag){
                    time(&pause_start_time);
                }
                running_flag=false;
                show_calendar();
                nodelay(stdscr, TRUE);
                //출결 기록을 띄우고 나면 화면을 다시 구성해야 함.
                erase();
                move(3,((max_x-15)/2)+6);
                addstr(display_time(hours, minutes, seconds));
                
                move(4, ((max_x-18)/2)+14);     addstr("pause!");

                move(2, (max_x-14)/2);     addstr("Pomodoro mode");
                move(5, (max_x-23)/2);     addstr("Start-Please enter '1'");
                move(6, (max_x-23)/2);     addstr("Pause-Please enter '2'");
                move(7, (max_x-24)/2);     addstr("Finish-Please enter '3'");
                if (work_period_flag) {//일하는 중이면
                    move(3, (max_x-17)/2);
                    printw("Work: Set %d/%d ",set_counter + 1, goal_sets);
                    move(4, ((max_x-18)/2)-4);
                    printw("remaining %02d:%02d",minutes, seconds); 
                } else {//휴식 중이면
                    move(3, (max_x-17)/2);
                    printw("Rest: Set %d/%d ",set_counter + 1, goal_sets);
                    move(4, ((max_x-18)/2)-4);
                    printw("remaining %02d:%02d",minutes, seconds); 
                }
                menu_bar_display
            }
            else if (command == '3'||command=='H'||command=='h'|| command=='S'||command=='s'||command=='G'||command=='g'){
                if(work_period_flag){//공부를 하던 중 3이 입력되면 지금까지의 공부 시간을 기록
                    total_elapsed_seconds+=elapsed_seconds;
                }
                running_flag=false;
                in_pomo_flag = false;
            }
        }

        if (running_flag) {
            if (remaining_seconds <= 0) {//해당 세트에 남은 시간이 없으면
                if (!work_period_flag && remaining_seconds==0) {//일+쉬는시간이 다 끝나면 set_counter를 증가시킨다.
                    set_counter++;
                }else if(work_period_flag&&remaining_seconds==0){ //일이 끝났으면 total 공부시간에 저장한다.
                    total_elapsed_seconds+=work_duration;
                }
                pause_time=0;//공부중->쉬는시간, 쉬는시간->공부중으로 바뀌면 총 일시중지 시간도 초기화 
                time(&start_time);
                work_period_flag = !work_period_flag; 
                //remaining_seconds가 0이 되면 work_period_flag의 bool값이 바뀜. 즉 휴식 모드로 들어감
                remaining_seconds = work_period_flag ? work_duration : rest_duration; 
                //work_period_flag가 참이면 남은 시간은 work_duration이고, 거짓이면 rest_duration임
                current_duration = work_period_flag ? work_duration : rest_duration;
                
                //beep();
                if (!work_period_flag && remaining_seconds==0) {
                    set_counter++;
                }
            }

            time(&current_time);
            elapsed_seconds = difftime(current_time, start_time) - pause_time;
            remaining_seconds = current_duration - elapsed_seconds;

            minutes = ((int)remaining_seconds % 3600) / 60;
            seconds = (int)remaining_seconds % 60;

            if (remaining_seconds <= 0 && goal_sets-1 == set_counter) {
                total_elapsed_seconds+=work_duration;
                remaining_seconds = 0;
                in_pomo_flag = false;
            //     beep();
            }

            move(3, 20);
            if (work_period_flag) {//일하는 중이면
                move(3, (max_x-17)/2);
                printw("Work: Set %d/%d ",set_counter + 1, goal_sets);
                move(4, ((max_x-18)/2)-4);
                printw("remaining %02d:%02d",minutes, seconds); 
            } else {//휴식 중이면
                move(3, (max_x-17)/2);
                printw("Rest: Set %d/%d ",set_counter + 1, goal_sets);
                move(4, ((max_x-18)/2)-4);
                printw("remaining %02d:%02d",minutes, seconds); 
            }
            refresh();
        }
        usleep(100000);
    }

    
    nodelay(stdscr, FALSE);
    
    //TODO: 여기서 total_elapsed_seconds 유저 아이디와 함께 서버로 전송
    if(command == '3'||command=='s'||command=='S'){
        start_study();
    }else if(command=='H'||command=='h'){
        mypage();
    }else if(command=='G'||command=='g'){
        manage_groups();
    }
}