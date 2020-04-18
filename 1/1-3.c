#include "apue.h"
#include <dirent.h>
//ls功能
int main(int argc,char *argv[])
{
	DIR	*dp;
	struct dirent 	*dirp;
	if(argc !=2)
		err_quit("usage: ls directory_name");
	if((dp=opendir(argv[1]))==NULL)//打开目录文件
		err_sys("can't open %s",argv[1]);
	while ((dirp=readdir(dp))!=NULL)//打印文件列表
		printf("%s\n",dirp->d_name);

	closedir(dp);
	exit(0);
}
