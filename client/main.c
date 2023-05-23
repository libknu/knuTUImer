#include "client.h"

int main(int argc, char **argv){
	int sock_fd;
	sock_fd = setup(argc, argv);
    get_terminal_size();

    //TODO: 파일 입출력 구현 이후 dummy_data.c 파일 삭제
    make_dummy_data();
    
    first_page();
    //manage_groups();
}