#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

sigset_t mask;
int lockfile(int fd)
{
    struct flock f1;
    f1.l_type = F_WRLCK;
    f1.l_start = 0;
    f1.l_whence = SEEK_SET;
    f1.l_len = 0;
    return fcntl(fd,F_SETLK,&f1);
}
int already_running(void)
{
    int     fd;
    char    buf[16];

    fd = open(LOCKFILE,O_RDWR|O_CREAT,LOCKMODE);
    if(fd < 0)
    {
        syslog(LOG_ERR,"can't open %s : %s",LOCKFILE,strerror(errno));
        exit(1);
    }
    if(lockfile(fd)<0)
    {
        if(errno == EACCES | errno == EAGAIN)
        {
            close(fd);
            return 1;
        }
        syslog(LOG_ERR,"can,t lock %s : %s",LOCKFILE,strerror(errno));
        exit(1);
    }
    ftruncate(fd,0);
    sprintf(buf,"%ld",(long)getpid());
    write(fd,buf,strlen(buf)+1);
    return 0;
}

void daemonize(const char *cmd)
{
    int                 i,fd0,fd1,fd2;
    pid_t               pid;
    struct rlimit       r1;
    struct sigaction    sa;
    umask(0);
    getrlimit(RLIMIT_NOFILE,&r1);
    if((pid = fork()) < 0)
    {
         perror("fork() error");
         exit(0);
    }
    else if(pid > 0)
        exit(0);
    setsid();
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP,&sa,NULL);
    if((pid = fork()) < 0)
    {
         perror("fork() error");
         exit(0);
    }
    else if(pid > 0)
        exit(0);
    chdir("/");
    if(r1.rlim_max == RLIM_INFINITY)
        r1.rlim_max = 1024;
    for(i=0;i<r1.rlim_max;++i)
        close(i);
    fd0 = open("/dev/null",O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    openlog(cmd,LOG_CONS,LOG_DAEMON);
    if(fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR,"unexpected file descriptors %d %d %d",fd0,fd1,fd2);
        exit(1);
    }
}

void reread()
{
    printf("read daemon config file again.\n");
}
void * thread_func(void *arg)
{
    int err,signo;
    while(1)
    {
        sigwait(&mask,&signo);
        switch(signo)
        {
        case SIGHUP:
            syslog(LOG_INFO,"Re-reading configuration file.\n");
            reread();
            break;
        case SIGTERM:
            syslog(LOG_INFO,"got SIGTERM;exiting.\n");
            exit(0);
        default:
            syslog(LOG_INFO,"unexpected signal %d.\n",signo);
        }
    }
    return NULL;
}
int main(int argc,char *argv[])
{
    pthread_t           tid;
    char                *cmd;
    struct sigaction    sa;
    if((cmd = strrchr(argv[0],'/')) == NULL)
        cmd = argv[0];
    else
        cmd++;
    daemonize(cmd);
    if(already_running())
    {
          syslog(LOG_ERR,"daemon already running.\n");
          exit(1);
    }
    sa.sa_handler =SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP,&sa,NULL);
    sigfillset(&mask);
    pthread_sigmask(SIG_BLOCK,&mask,NULL);
    pthread_create(&tid,NULL,thread_func,0);
    sleep(90);
    exit(0);
}