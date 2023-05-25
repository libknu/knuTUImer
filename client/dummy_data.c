#include "client.h"
#include <string.h>
#include <stdio.h>
//프론트 구현을 위한 더미 데이터로, 실제로는 링크드리스트로 구현되며 파일 입출력이 필요함
//TODO: 이 파일은 제출시 제외함

tg dummy_groups[10];
tm dummy_members[20];

void make_dummy_data(){
    
    //가상의 유저가 속한 그룹의 더미데이터를 만듦. 10개라고 가정
    for(int i=0;i<10;i++){
        char groupname[20];
        sprintf(groupname,"testgroupname[%d]",i);
        strcpy(dummy_groups[i].group_name, groupname);

        dummy_groups[i].study_time=100*(i+1);
    }

    //가상의 그룹에 속해있는 유저의 더미데이터를 만듦. 20명이라고 가정
    for(int i=0;i<20;i++){
        char username[20];
        sprintf(username,"testusername[%d]",i);
        strcpy(dummy_members[i].user_name, username);

        dummy_members[i].study_time=100*(i+1);
    }

}
