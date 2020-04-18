#include "apue.h"

#define BUFFSIZE  4096
//无缓冲区读取标准输入，写入标准输出
int 
main (void)
{
	int n;
	char buf[BUFFSIZE];
	
	while ((n=read(STDIN_FILENO,buf,BUFFSIZE))>0)
		if(write(STDOUT_FILENO,buf,n)!=n)
			err_sys("write error");
	if(n<0)
		err_sys("read error");
	
	exit(0);
}