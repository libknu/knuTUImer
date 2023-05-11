#include<stdio.h>
#include<stdlib.h>
#include<curses.h>
#include<string.h>
#include<unistd.h>
#include <time.h>

//gcc index.c -o index -lcurses


struct calendarInfo
{
    int number_of_days_in_month;//이번 달은 며칠짜리?
    int start_day;//이번 달은 무슨 요일부터?
};

const static char MON[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

void first_page();//첫 시작 페이지. 로그인 또는 회원가입
void login();//로그인
void creatID();//회원가입
void mypage();//show_calendar(하루 중 1회만. 4초간 보여줌.)/Today’s Total Focusing(보여만 줌)/ 공부 시작하기(stdin(1)) / 그룹 관리하기(stdin(2))-goole docs에서 바뀐 부분
struct calendarInfo get_calender_info(struct tm);
void show_calendar(); //하루 중 첫 접속때만 first page에서 호출
char* display_time(int hour, int minute, int second);
void start_study(); //Choose mode(스톱워치(stdin(1)), 타이머(stdin(2)), 뽀모도로(stdin(3)))
void stopwatch_mode();
void timer_mode();
void pomodoro_mode();
void manage_groups(); //Group List(stdin(1)) Join other Group(stdin(2)) Make my Group(stdin(3))
void group_list();
bool is_exist_member(char[]);
void join_other_Group();
void make_my_Group();

int main(void){
    //first_page();
    //login();
    //creatID();
    //stopwatch_mode();
    //timer_mode();
    pomodoro_mode();
}

void first_page(){
    //첫 접속 시 나타나는 창이다. 로그인을 하거나, 회원가입을 한다.
    int command;
    
    /***************화면 구성***************/
    initscr();
    noecho();
    start_color();
    erase();// 화면 내용을 다 지움
    move(2, 2);    addstr("log in-Please enter '1'");
    move(3, 2);    addstr("sign up-Please enter '2'");
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
        move(2, 2);     addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        first_page();
    }
    /****************************************/
    endwin();
}

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
    move(2, 2);
    addstr("Log in");

    move(4, 2);     addstr("ID: ");
    move(5, 2);     addstr("Password: ");
    move(4, 6); //커서를 ID에  맞게 이동
    refresh();

    //https://stackoverflow.com/questions/50014137/printw-plus-scanf-in-ncurse-window-wont-display-typed-text
    curs_set(1);
    nodelay(stdscr, FALSE);
    echo();
    scanw("%s", id);
    nodelay(stdscr, TRUE);
    noecho();
    refresh();

    //비밀번호에 맞게 이동
    move(5, 12);
    refresh();

    nodelay(stdscr, FALSE);
    echo();
    scanw("%s", password);
    nodelay(stdscr, TRUE);
    noecho();
    refresh();
    endwin();

    if((strcmp(id,testID)!=0)||(strcmp(password,testPassword)!=0)){//만약 아이디나 패스워드가 일치하지 않으면
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);     
        // printw("debug: [%s][%s]",id,password);
        addstr("The ID or password doesn't match.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        login();//로그인 다시 시작
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

    move(2, 2);     addstr("Sign up");
    move(4, 2);     addstr("ID: ");
    move(5, 2);     addstr("Password: ");
    //디스플레이 꾸미기 필요

    move(4, 6); //커서를 ID에  맞게 이동
    refresh();
    curs_set(1);//일반 커서를 보여줌

    nodelay(stdscr, FALSE);
    echo();
    scanw("%s", id);
    nodelay(stdscr, TRUE);
    noecho();
    refresh();
    endwin();


    //비밀번호에 맞게 이동
    move(5, 12);
    refresh();

    nodelay(stdscr, FALSE);
    echo();
    scanw("%s", password);
    nodelay(stdscr, TRUE);
    noecho();
    refresh();
    endwin();

    if((strcmp(id,testID)==0)){//만약 아이디가 중복이면
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);     addstr("This ID is already exist.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        creatID();//아이디 만들기 다시 시작
    }else{//로그인 성공
        first_page();
    }

}

void mypage(){
    int First_access_flag=1;//TODO: 디버깅용으로 1로 설정하였으며, 실제로는 그 날의 첫 접속인지 확인해야 함
    int command;

    if(First_access_flag==1){//하루 중 첫 접속이면
        show_calendar();
    }

    /***************화면 구성***************/
    initscr();
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움
    move(2, 2);     addstr("Today's Total Focusing");
    move(3, 2);     addstr(display_time(5,0,0));//TODO: 실제로는 총 공부 시간을 파일에서 읽어오는 함수를 새로 만들어야 함. 
    move(4, 2);     addstr("Start studying-Please enter '1'");
    move(5, 2);     addstr("Manage groups-Please enter '2'");
    refresh();  
    /****************************************/

    /***************커맨드 입력***************/
    // rewind(stdin);
    nodelay(stdscr, FALSE);
    command = getch();
    if(command=='1'){        
        start_study();
    }else if(command=='2'){
        manage_groups();
    }else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);
        //printw("%c",command);
        addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        mypage();
    }  
    endwin();
}

struct calendarInfo get_calender_info(struct tm tm) {
    struct calendarInfo info;
    int month = tm.tm_mon;
    int year = tm.tm_year + 1900;

    //총 며칠인지 계산
    if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11) {
        info.number_of_days_in_month = 31;
    } else if (month == 1) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            info.number_of_days_in_month = 29;
        } else {
            info.number_of_days_in_month = 28;
        }
    } else {
        info.number_of_days_in_month = 30;
    }

    //무슨 요일부터 시작하는지 계산. 첫째날의 정보를 담은 새로운 tm인 first_day_of_month를 만든다.
    time_t t = time(NULL);
    struct tm first_day_of_month = *localtime(&t);

    first_day_of_month.tm_mday = 1;
    first_day_of_month.tm_mon = month;
    first_day_of_month.tm_year = year - 1900;
    mktime(&first_day_of_month);

    //first_day_of_month의 요일 정보를 저장.
    info.start_day = first_day_of_month.tm_wday;
    // printf("%d",first_day_of_month.tm_wday);

    return info;
}

void show_calendar(){ //하루 중 첫 접속때만 first page에서 호출
    
    //TODO: 출석 정보 저장
    
    //내가 생각하고, 임시로 디버깅해본 방법은 다음과 같음
    //날짜(日)를 저장하는 arr 배열을 만든다.(또, 실제로는 파일에 입력해야 함. 나는 日만 고려했지만, 月도 고려해야 함) 
    //오늘 날짜(日)를 불러와서, 해당 arr(日)=1; 한다.
    //arr을 순회하면서 arr[日]==1인 날만 날짜를 반전시켜 보여준다.

    int arr[32] = {0,};
    int attended_day;
    int current_day=1;
    int row = 2; //0,1은 헤더가 사용중이기 때문
    int col;

    //오늘 날짜를 불러와서 time_t에 저장
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    //오늘의 日을 1로 함
    arr[tm.tm_mday] = 1;

    initscr();
    clear();
    curs_set(0); //커서를 안보이게 함

    //색상 팔레트 만들기: https://m.blog.naver.com/guile21c/53287736
    init_pair(1, 0, 7);
    // init_pair(2, COLOR_BLACK,COLOR_WHITE);

    //헤더- 달 출력
    move(0,9);
    addstr(MON[tm.tm_mon]);

    //헤더-요일 출력
    move(1,0);
    addstr("Su Mo Tu We Th Fr Sa");

    struct calendarInfo info=get_calender_info(tm);


    /*****************달력 그리기*******************/
    while (current_day <= info.number_of_days_in_month) {//current_day(지금 출력하고 있는 day)가 이번 달의 마지막 날일때 까지
        for (col = info.start_day ;  col < 7 && current_day <= info.number_of_days_in_month; col++) { 
            //시작 요일부터 6(일요일)까지 출력 후 줄바꿈하고 그 뒤로는 한 행에 7일씩 출력. 
            char day_str[3];
            snprintf(day_str, sizeof(day_str), "%2d", current_day); //day를 형식에 맞게 저장
            move(row, col * 3);
            if (arr[current_day] == 1) { //arr[日]이 1인 경우는 색상을 반전시켜 출력                
                attron(COLOR_PAIR(1));
                addstr(day_str);
                attroff(COLOR_PAIR(1));
            } else {
                addstr(day_str);
            }
            current_day++;
        }
        info.start_day = 0;
        row++;
    }

    refresh();
    sleep(3); //만든 달력을 3초간 보여줌
    endwin();
}

char* display_time(int hour, int minute, int second){
    //TODO: 실제로는 시, 분, 초에 적당한 값을 인자로 받아와야 함
	static char str[50];
    // int hour=5;
    // int minute=0;
    // int second=0;
    sprintf(str,"%02d:%02d:%02d", hour, minute, second);
	return str;
}

void start_study(){ //Choose mode(스톱워치(stdin(1)), 타이머(stdin(2)), 뽀모도로(stdin(3)))

    int command;
    
    /***************화면 구성***************/
    initscr();
    curs_set(0);
    clear();
    move(2,2);      addstr("Choose an option:\n");
    move(3,2);      addstr("1. Timer\n");
    move(4,2);      addstr("2. Stopwatch\n");
    move(5,2);      addstr("3. Pomodoro\n");
    refresh();
    /****************************************/

    /****************커맨드 입력****************/
    // getchar();
    command = getch();
    if(command=='1'){        
        timer_mode();
    }else if(command=='2'){
        stopwatch_mode();
    }else if(command=='3'){
        pomodoro_mode();
    }
    else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);     addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        start_study();
    }
    /****************************************/
}

void stopwatch_mode(){ //time up
    
    int command;
    bool running_flag = false;
    bool in_stopwatch_flag = true;
    bool start_flag=true;
    time_t start_time, current_time, pause_start_time, pause_end_time;
    double pause_time;//휴식 시간의 총합
    double elapsed_seconds;//현재시간-시작시간-휴식시간
    int hours, minutes, seconds;

    /***************화면 구성***************/
    initscr();
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움
    move(2, 2);     addstr("Stopwatch mode");
    move(4, 2);     addstr("Start-Please enter '1'");
    move(5, 2);     addstr("Pause-Please enter '2'");
    move(6, 2);     addstr("Finish-Please enter '3'");
    refresh();  
    /****************************************/
    
    
    nodelay(stdscr, TRUE);
    
    if(start_flag == true){
        move(3,2);
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
                move(3, 20);
                addstr("                     ");
                move(3, 20);
                addstr("running...");
            }
            else if (command == '2' && running_flag){ // 휴식 시작
                time(&pause_start_time);
                running_flag = false;
                move(3, 20);
                addstr("                     ");
                move(3, 20);
                addstr("pause!");
            }
            else if (command == '3'){
                in_stopwatch_flag = false;
                break;
            }
        }
        move(3,2);

        if(running_flag){
            time(&current_time);
            elapsed_seconds = difftime(current_time, start_time) - pause_time;
            hours = (int)elapsed_seconds / 3600;
            minutes = ((int)elapsed_seconds % 3600) / 60;
            seconds = (int)elapsed_seconds % 60;
            addstr(display_time(hours, minutes, seconds));
            move(7, 2);
            refresh();
        }
        // printw("r: %lf e: %lf p: %lf",difftime(current_time, start_time),elapsed_seconds, pause_time);
        
        usleep(100000);//0.1초마다 순회
    }

    //여기서 elapsed_seconds 를 적당한 값으로(초로 변환?)해서 저장하면 될 듯
    endwin();
}

void timer_mode(){ //time down
    //TODO: 내부 기능 구현
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
    initscr();
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움
    move(2, 2);     addstr("Timer mode");
    move(3, 2);     addstr("Insert your goal!");
    move(4, 2);     addstr("Hours: ");
    move(5, 2);     addstr("Minutes: ");
    move(4, 9); //커서를 hour 에 맞게 이동
    refresh();
    curs_set(1);//일반 커서를 보여줌

    nodelay(stdscr, FALSE);
    echo();
    scanw("%d", &goal_hour);
    nodelay(stdscr, TRUE);
    noecho();
    refresh();

    //minutes에 맞게 이동
    move(5, 11);
    refresh();
    nodelay(stdscr, FALSE);
    echo();
    scanw("%d", &goal_minute);
    nodelay(stdscr, TRUE);
    noecho();
    refresh();
    endwin();

    erase();
    curs_set(0);
    move(2, 2);     addstr("Timer mode");
    move(3, 2);     addstr("Remaining time");

    // move(3, 20);    addstr(display_time(5,0,0));

    move(4, 2);     addstr("Start-Please enter '1'");
    move(5, 2);     addstr("Pause-Please enter '2'");
    move(6, 2);     addstr("Finish-Please enter '3'");
    refresh();  
    /****************************************/
    //TODO: 각 조건문마다 알맞은 시간 처리 필요


    // nodelay(stdscr, TRUE);

    if(start_flag == true){
        move(3,20);
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
                move(3, 30);
                addstr("                     ");
                move(3, 30);
                addstr("running...");
            }
            else if (command == '2' && running_flag){//작동 중지
                time(&pause_start_time);
                running_flag = false;
                move(3, 30);
                addstr("                     ");
                move(3, 30);
                addstr("pause!");
            }
            else if (command == '3'){
                in_timer_flag = false;
                break;
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

            move(3, 20);
            addstr(display_time(hours, minutes, seconds));
            refresh();
        }
        usleep(100000);
    }    
    endwin();
    //여기서 elapsed_seconds저장
}

void pomodoro_mode(){
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

    initscr();
    noecho();
    curs_set(0);
    erase();
    move(2, 2);     addstr("Pomodoro mode");
    move(3, 2);     addstr("Enter the number of sets: ");
    curs_set(1);

    nodelay(stdscr, FALSE);
    echo();
    scanw("%d", &goal_sets);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);

    erase();
    move(2, 2);     addstr("Pomodoro mode");
    move(5, 2);     addstr("Start-Please enter '1'");
    move(6, 2);     addstr("Pause-Please enter '2'");
    move(7, 2);     addstr("Finish-Please enter '3'");
    refresh();


    if(start_flag == true){ //처음 1을 입력하기 전에 표시할 내용
        move(3, 2);
        printw("Work: Set %d/%d ",1, goal_sets);
        move(4, 2);
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
                move(4, 20);
                addstr("                ");
                move(4, 20);
                addstr("running...");
                running_flag = true;
            } else if (command == '2' && running_flag) {//일시중지중. 공부중, 일하는 중 모두에서 작동
                time(&pause_start_time);
                running_flag = false;
                move(4, 20);
                addstr("                ");
                move(4, 20);
                addstr("pause!");
                running_flag = false;
            } else if (command == '3') {
                if(work_period_flag){//공부를 하던 중 3이 입력되면 지금까지의 공부 시간을 기록
                    total_elapsed_seconds+=elapsed_seconds;
                }
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
                move(3, 2);
                printw("Work: Set %d/%d ",set_counter + 1, goal_sets);
                move(4, 2);
                printw("remaining %02d:%02d",minutes, seconds); 
            } else {//휴식 중이면
                move(3, 2);
                printw("Rest: Set %d/%d ",set_counter + 1, goal_sets);
                move(4, 2);
                printw("remaining %02d:%02d",minutes, seconds); 
            }
            move(8,2);
            printw("%d",(int)total_elapsed_seconds);
            refresh();
        }
        usleep(100000);
    }
    endwin();
}

void manage_groups(){
    int command;
    
    /***************화면 구성***************/
    initscr();
    curs_set(0);
    clear();
    move(2,2);
    addstr("Choose an option:\n");
    move(3,2);
    printw("1. Group List\n");
    move(4,2);
    addstr("2. Join other Group\n");
    move(5,2);
    addstr("3. Make my Group\n");
    refresh();
    /****************************************/

    /****************커맨드 입력****************/
    command = getch();
    if(command=='1'){        
        group_list();
    }else if(command=='2'){
        join_other_Group();
    }else if(command=='3'){
        make_my_Group();
    }
    else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);
        addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        manage_groups();
    }
    /****************************************/
}
void group_list(){
    //여기는 나중에 반복문으로 유저 목록을 출력해야 하는데, 현재상태로는 어려워서 단순출력함.
    //구조체 멤버가 정해지면 반복문 형태로 수정
    int command;
    int i;
    char ch;
    char member_to_call[20];
    initscr();
    noecho();
    erase();

    move(2, 2);
    addstr("Group List");
    move(3, 2);
    printw("%s's total focusing time %02d:%02d","member1",5, 20); 
    move(4,2);
    printw("%s's total focusing time %02d:%02d","member2",5, 20); 
    refresh();

    move(5,2);
    addstr("Enter the ID of the member you want to call: ");
    move(5, 47);

    nodelay(stdscr, FALSE);
    echo();
    scanw("%s", member_to_call);
    nodelay(stdscr, TRUE);
    noecho();
    refresh();
    endwin();

    if(is_exist_member(member_to_call)){
        //TODO: 여기서 통신 필요. 
        move(6,2);
        printw("You called %s!",member_to_call);
    }else{
        move(6,2);
        printw("%s doesn't exist.",member_to_call);
    }
    refresh();
    sleep(1);
    endwin();
    group_list();
}

bool is_exist_member(char name[]){
    //TODO: 멤버 리스트를 순회하면서, 입력받은 이름의 유저가 있는지 찾는다.
    return true;
}
void join_other_Group(){
    initscr();
    erase();
    refresh();
    endwin();
}
void make_my_Group(){
    initscr();
    erase();
    refresh();
    endwin();
}