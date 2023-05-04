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
char* display_time();
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
    //stopwatch_mode();
    //timer_mode();
    //pomodoro_mode();
    //first_page();
    group_list();
}

void first_page(){
    //첫 접속 시 나타나는 창이다. 로그인을 하거나, 회원가입을 한다.
    int command;
    
    /***************화면 구성***************/
    initscr();
    noecho();
    start_color();
    erase();// 화면 내용을 다 지움
    move(2, 2);
    addstr("log in-Please enter '1'");
    move(3, 2);
    addstr("sign up-Please enter '2'");
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
        move(2, 2);
        addstr("Please enter the correct key.");
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

    move(4, 2);
    addstr("ID: ");
    move(5, 2);
    addstr("Password: ");
    move(4, 6); //커서를 ID에  맞게 이동
    refresh();
    curs_set(1);//일반 커서를 보여줌
    //이 방식으로 처리하는 이유는, 입력한 키를 실시간으로 보여주기 위함임
    //scanf이용 시 엔터를 치기 전까지 내가 입력한 키를 화면에 업데이트 하지 않음
    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리. 127은 백스페이스 키의 아스키 넘버임
            if (i > 0) {
                i--;
                move(4, 6 + i);
                addch(' ');
                move(4, 6 + i);
            }
        } else {
            id[i++] = ch;
            addch(ch);
        }
        refresh();
    }
    id[i] = '\0';

    //비밀번호에 맞게 이동
    move(5, 12);
    refresh();

    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리
            if (i > 0) {
                i--;
                move(5, 12 + i);
                addch(' ');
                move(5, 12 + i);
            }
        } else {
            password[i++] = ch;
            addch('*'); //ID와 달리 화면에 보여주는 것은 입력된 문자가 아니라 *임
        }
        refresh();
    }
    password[i] = '\0';
    endwin();

    if((strcmp(id,testID)!=0)||(strcmp(password,testPassword)!=0)){//만약 아이디나 패스워드가 일치하지 않으면
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);
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

    move(2, 2);
    addstr("Sign up");
    move(4, 2);
    addstr("ID: ");
    move(5, 2);
    addstr("Password: ");
    //디스플레이 꾸미기 필요

    move(4, 6); //커서를 ID에  맞게 이동
    refresh();
    curs_set(1);//일반 커서를 보여줌

    //이 방식으로 처리하는 이유는, 입력한 키를 실시간으로 보여주기 위함임
    //scanf이용 시 엔터를 치기 전까지 내가 입력한 키를 화면에 업데이트 하지 않음
    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리. 127은 백스페이스 키의 아스키 넘버임
            if (i > 0) {
                i--;
                move(4, 6 + i);
                addch(' ');
                move(4, 6 + i);
            }
        } else {
            id[i++] = ch;
            addch(ch);
        }
        refresh();
    }

    id[i] = '\0';


    //비밀번호에 맞게 이동
    move(5, 12);
    refresh();

    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리
            if (i > 0) {
                i--;
                move(5, 12 + i);
                addch(' ');
                move(5, 12 + i);
            }
        } else {
            password[i++] = ch;
            addch('*'); //ID와 달리 화면에 보여주는 것은 입력된 문자가 아니라 *임
        }
        refresh();
    }
    password[i] = '\0';
    endwin();

    if((strcmp(id,testID)==0)){//만약 아이디가 중복이면
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);
        addstr("This ID is already exist.");
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
    move(2, 2);
    addstr("Today's Total Focusing");
    move(3, 2);
    addstr(display_time());//TODO: 실제로는 총 공부 시간을 파일에서 읽어오는 함수를 새로 만들어야 함. 
    move(4, 2);
    addstr("Start studying-Please enter '1'");
    move(5, 2);
    addstr("Manage groups-Please enter '2'");
    refresh();  
    /****************************************/

    /***************커맨드 입력***************/
    command = getch();

    if(command=='1'){        
        start_study();
    }else if(command=='2'){
        manage_groups();
    }else{
        initscr();
        erase();// 화면 내용을 다 지운 뒤
        move(2, 2);
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
    while (current_day <= info.number_of_days_in_month) {
        for (col = info.start_day ;  col < 7 && current_day <= info.number_of_days_in_month; col++) {
            char day_str[3];
            snprintf(day_str, sizeof(day_str), "%2d", current_day);
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
    sleep(3);
    // clear();
    endwin();
}

char* display_time(){
    //TODO: 실제로는 시, 분, 초에 적당한 값을 인자로 받아와야 함
	static char str[50];
    int hour=5;
    int minute=0;
    int second=0;
    sprintf(str,"%02d:%02d:%02d", hour, minute, second);
	return str;
}

void start_study(){ //Choose mode(스톱워치(stdin(1)), 타이머(stdin(2)), 뽀모도로(stdin(3)))

    int command;
    
    /***************화면 구성***************/
    initscr();
    curs_set(0);
    clear();
    move(2,2);
    addstr("Choose an option:\n");
    move(3,2);
    printw("1. Timer\n");
    move(4,2);
    addstr("2. Stopwatch\n");
    move(5,2);
    addstr("3. Pomodoro\n");
    refresh();
    /****************************************/

    /****************커맨드 입력****************/
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
        move(2, 2);
        addstr("Please enter the correct key.");
        refresh();
        sleep(1);//1초간 오류를 보여주고
        endwin();
        start_study();
    }
    /****************************************/
}

void stopwatch_mode(){ //time up
    //TODO: 내부 기능 구현
    
    int command;
    bool running_flag = false;
    bool in_stopwatch_flag = true;
    
    /***************화면 구성***************/
    initscr();
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움
    move(2, 2);
    addstr("Stopwatch mode");
    move(4, 2);
    addstr("Start-Please enter '1'");
    move(5, 2);
    addstr("Pause-Please enter '2'");
    move(6, 2);
    addstr("Finish-Please enter '3'");
    refresh();  
    /****************************************/

    while (in_stopwatch_flag) {
        command = getch();
        //각 조건에 적당한 처리를 해 주어야 함

        if (command == '1' && !running_flag) {//작동중
            running_flag = true;
            move(3,20);
            addstr("                     ");
            move(3,20);
            addstr("running...");
        } else if (command == '2' && running_flag) {//일시정지중
            running_flag = false;
            move(3,20);
            addstr("                     ");
            move(3,20);
            addstr("pause!");
        } else if (command == '3') {
            in_stopwatch_flag = false;
            break;
        }

        if (running_flag) {
            //1초 단위로 if문을 실행해서 적당한 h,m,s를 구한 뒤
            //display_time(h,m,s);를 인자로 주면 됨.
            //전체 진행시간-멈춘시간으로 구해야 함
            move(3,2);
            addstr(display_time());
        }
         usleep(100000);
    }
    
    //여기서 h,m,s를 적당한 값으로(초로 변환?)해서 저장하면 될 듯
    endwin();
}

void timer_mode(){ //time down
    //TODO: 내부 기능 구현
    int command;
    int goal_hour, goal_minute;
    char temp_hour[10];
    char temp_minute[10];
    char ch;
    int i;
    bool running_flag = false;
    bool in_timer_flag = true;

    /***************화면 구성***************/
    initscr();
    noecho();
    curs_set(0);//커서를 안보이게 설정
    erase();// 화면 내용을 다 지움
    move(2, 2);
    addstr("Timer mode");
    move(3, 2);
    addstr("Insert your goal!");
    move(4, 2);
    addstr("Hours: ");
    move(5, 2);
    addstr("Minutes: ");

    move(4, 9); //커서를 hour 에 맞게 이동
    refresh();
    curs_set(1);//일반 커서를 보여줌

    //이 방식으로 처리하는 이유는, 입력한 키를 실시간으로 보여주기 위함임
    //scanf이용 시 엔터를 치기 전까지 내가 입력한 키를 화면에 업데이트 하지 않음
    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리. 127은 백스페이스 키의 아스키 넘버임
            if (i > 0) {
                i--;
                move(4, 9 + i);
                addch(' ');
                move(4, 9 + i);
            }
        } else {
            temp_hour[i++] = ch;
            addch(ch);
        }
        refresh();
    }
    temp_hour[i] = '\0';

    //minutes에 맞게 이동
    move(5, 11);
    refresh();
    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리
            if (i > 0) {
                i--;
                move(5, 11 + i);
                addch(' ');
                move(5, 11 + i);
            }
        } else {
            temp_minute[i++] = ch;
            addch(ch);
        }
        refresh();
    }
    temp_minute[i] = '\0';
    endwin();


    erase();
    curs_set(0);
    move(2, 2);
    addstr("Timer mode");
    move(3, 2);
    addstr("Remaining time");
    move(3, 20);
    addstr(display_time());

    move(4, 2);
    addstr("Start-Please enter '1'");
    move(5, 2);
    addstr("Pause-Please enter '2'");
    move(6, 2);
    addstr("Finish-Please enter '3'");
    refresh();  
    /****************************************/
    //TODO: 각 조건문마다 알맞은 시간 처리 필요 
    while (in_timer_flag) {
        command = getch();

        if (command == '1' && !running_flag) {
            move(3,30);
            addstr("                     ");
            move(3,30);
            addstr("running...");
            running_flag = true;
        } else if (command == '2' && running_flag) {
            move(3,30);
            addstr("                     ");
            move(3,30);
            addstr("pause!");
            running_flag = false;
        }else if (command == '3') {
            in_timer_flag = false;
            break;
        }

        if (running_flag) {

            // if (remaining_seconds <= 0) {
            //     remaining_seconds = 0;
            //     in_timer_flag = false;
            //     beep();
            // }

            move(3,20);
            addstr(display_time());
        }
         usleep(100000);
    }
    endwin();

    //여기서 남은 시간 저장
}

void pomodoro_mode(){

    char temp_set[4];
    int n_sets;
    int i;
    char ch;
    bool running_flag = false; //포모도로 시간이 흐르는 중인지 확인하는 플래그
    bool in_pomo_flag = true;  //포모도로 타이머 자체가 작동중인지 확인하는 플래그. false면 함수가 끝남
    int set_counter = 0;

    int work_duration = 25 * 60;
    int rest_duration = 5 * 60;

    int remaining_seconds = work_duration;
    bool work_period_flag = true; //25분 사이클 안에 있는지 표시하는 플래그

    initscr();
    noecho();
    curs_set(0);
    erase();
    move(2, 2);
    addstr("Pomodoro mode");
    move(3, 2);
    addstr("Enter the number of sets: ");
    curs_set(1);

    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리. 127은 백스페이스 키의 아스키 넘버임
            if (i > 0) {
                i--;
                move(4, 9 + i);
                addch(' ');
                move(4, 9 + i);
            }
        } else {
            temp_set[i++] = ch;
            addch(ch);
        }
        refresh();
    }
    temp_set[i] = '\0';
    n_sets=atoi(temp_set);
    curs_set(0);

    erase();
    move(2, 2);
    addstr("Pomodoro mode");
    move(5, 2);
    addstr("Start-Please enter '1'");
    move(6, 2);
    addstr("Pause-Please enter '2'");
    move(7, 2);
    addstr("Finish-Please enter '3'");
    refresh();


    while (in_pomo_flag && set_counter < n_sets) {
        ch = getch();
        if (ch == '1' && !running_flag) {
            move(4, 20);
            addstr("                ");
            move(4, 20);
            addstr("running...");
            running_flag = true;
        } else if (ch == '2' && running_flag) {
            move(4, 20);
            addstr("                ");
            move(4, 20);
            addstr("pause!");
            running_flag = false;
        } else if (ch == '3') {
            in_pomo_flag = false;
        }

        if (running_flag) {
            if (remaining_seconds <= 0) {
                work_period_flag = !work_period_flag; //remaining_seconds가 0이 되면 work_period_flag의 bool값이 바뀜
                remaining_seconds = work_period_flag ? work_duration : rest_duration; //work_period_flag가 참이면 남은 시간은 work_duration이고, 거짓이면 rest_duration임
                beep();
                if (!work_period_flag) {
                    set_counter++;
                }
            }

            if (work_period_flag) {
                move(3, 2);
                printw("Work: Set %d/%d ",set_counter + 1, n_sets);

                move(4, 2);
                //임의로 넣었지만, 실제 시간을 계산해야 함
                printw("remaining %02d:%02d",5, 20); 
            } else {
                move(3, 2);
                printw("Rest: Set %d/%d ",set_counter + 1, n_sets);

                move(4, 2);
                //임의로 넣었지만, 실제로는 시간을 계산해야 함
                printw("remaining %02d:%02d",5, 20); 
            }
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

    i = 0;
    while ((ch = getch()) != '\n') {
        if (ch == 127) { // Backspace 처리. 127은 백스페이스 키의 아스키 넘버임
            if (i > 0) {
                i--;
                move(5, 47 + i);
                addch(' ');
                move(5, 47 + i);
            }
        } else {
            member_to_call[i++] = ch;
            addch(ch);
        }
        refresh();
    }
    member_to_call[i] = '\0';

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