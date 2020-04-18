#include "apue.h"
//打印进程id
int 
main (void)
{	
	printf("hello world from process ID %ld\n",(long)getpid());
	exit(0);
}