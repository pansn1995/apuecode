#include "apue.h"
//有缓冲的
int 
main (void)
{
	int c;
	
	while ((c=getc(stdin))!=EOF)
		if(putc(c,stdout)==EOF)
			err_sys("output error");
	if(ferror(stdin))
		err_sys("input error");
	
	exit(0);
}