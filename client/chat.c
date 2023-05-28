#include "client.h"
#include "chat.h"
#include <string.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
	

extern max_x, max_y, menu_bar;
typedef struct {
    char message[MESSAGE_SIZE];
    char user[USER_NAME_SIZE];
    char group[GROUP_NAME_SIZE];
} PACKET;

void * send_packet(void * arg);
void * receive_packet(void * arg);

PACKET recv_packet;	
PACKET snd_packet;

//실제로는 유저와 그룹을 인자로 받아와야 함
char user[USER_NAME_SIZE]="duser";
char group[GROUP_NAME_SIZE]="dgroup";

extern max_x, max_y, menu_bar;

//TODO: 서버와 통신 및 인자로 유저 정보, 그룹 정보 받아오기
void group_chat(int fd){

    char message_for_view[MESSAGE_SIZE];
    char message_for_type[BUF_SIZE];
    erase();
    move(2, (max_x-12-strlen(group))/2);
    printw("[%s] Chat room", group);
    move(menu_bar,(max_x-76)/2); 
    addstr("                                                                                      ");
    menu_bar_message;
    move(menu_bar-1, (max_x-53)/2);
    printw("If you want to leave this chat room, type \"B\" or \"b\"");
    move(menu_bar-2, (max_x-53)/2); printw("Me: ");
    nodelay(stdscr, FALSE);
    getch();
    sleep(1);

    /***************************************************/
    // int sock;
	// struct sockaddr_in server_addr;
	// pthread_t send_thread, receive_thread;
	// void * thread_return;
	// // if(argc!=4) {
	// // 	printf("Usage : %s <IP> <port> <user name>\n", argv[0]);
	// // 	exit(1);
	// // }
	
	// printf("Group to enter: ");
	// scanf("%s",group);
	// sprintf(snd_packet.group, "%s", group);

	// // sprintf(user,"%s",argv[3]);
	// sprintf(snd_packet.user, "%s", user);


	// sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// memset(&server_addr, 0, sizeof(server_addr));
	// server_addr.sin_family=AF_INET;
	// server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	// server_addr.sin_port=htons(atoi(10000));
	  
	// if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))==-1)
	// 	oops("connect");
	
	// //패킷을 보내는 쓰레드
	// if(pthread_create(&send_thread, NULL, send_packet, (void*)&sock)==-1)
	// 	oops("pthread_create");

	// //패킷을 받는 쓰레드
	// if(pthread_create(&receive_thread, NULL, receive_packet, (void*)&sock)==-1)
	// 	oops("pthread_create");

	// pthread_join(send_thread, &thread_return);
	// pthread_join(receive_thread, &thread_return);

	// close(sock);  
	// return 0;
    /***************************************************/
}

// void * send_packet(void * arg)//패킷을 보내는 쓰레드
// {
// 	int sock=*((int*)arg);

// 	getchar();
// 	while(1) 
// 	{
// 		fgets(snd_packet.message, BUF_SIZE, stdin);
// 		// printf("debug: [%s] %s", snd_packet.user, snd_packet.message);

// 		if(!strcmp(snd_packet.message,"q\n")||!strcmp(snd_packet.message,"Q\n")){
// 			close(sock);
// 			exit(0);
// 		}
// 		write(sock, &snd_packet, sizeof(snd_packet));
// 		memset(snd_packet.message, 0, strlen(snd_packet.message));
// 	}
// 	return NULL;
// }

	
// void * receive_packet(void * arg) //패킷을 받는 쓰레드
// {
// 	int sock=*((int*)arg);
// 	int str_len;
// 	char message[BUF_SIZE];
// 	while(1)
// 	{
// 		read(sock, &recv_packet, sizeof(recv_packet));
// 		if(str_len==-1) 
// 			return (void*)-1;

// 		if(strcmp(group,recv_packet.group)==0){
// 			sprintf(message,"[%s] %s",recv_packet.user, recv_packet.message);
//             fputw(message, stdout);
// 		}
// 	}
// 	return NULL;
// }