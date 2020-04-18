#include "apue.h"
//判断是不是管道 FIFO 或套接字
int
main(void)
{
	if(lseek(STDIN_FILENO,0,SEEK_CUR)==-1)
		printf("cannot seek\n");
	else
		printf("seek OK\n");
	exit(0);
}