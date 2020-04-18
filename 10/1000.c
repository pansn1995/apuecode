#include "apue.h"

void sig_abort(int signo)
{
	printf("jinrula\n");
	exit(0);
}


void main()
{
	signal(	SIGABRT,&sig_abort);
	
	abort();
	pause();
	exit(0);
}