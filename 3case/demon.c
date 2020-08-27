#include <stdio.h>       
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h> 

void daemonise()
{
    pid_t pid=fork();

    if(pid==-1)
    {
        printf("Не удалось выполнить fork для демониизации");
        exit(-1);
    }
    if(pid!=0)
        exit(0);

    if(setsid()==-1){
        printf("Не удалось стать лидером сеанса для демонизации");
        exit(-1);
    }

    signal(SIGHUP,SIG_IGN);
    pid=fork();
     if(pid==-1)
    {
        printf("Не удалось выполнить fork для демониизации");
        exit(-1);
    }
    if(pid!=0)
        exit(0);
    
    if(chdir("/")==-1)
        printf("Не удалось установить корневой каталог при демонизации");

    umask(0);
    printf("pid %d", pid);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    if(open("/dev/null", O_RDONLY)==-1)
        printf("Не удалось открыть stdin для демонизации");
    if(open("/dev/null", O_WRONLY)==-1)
        printf("Не удалось открыть stdout для демонизации");
    if(open("/dev/null", O_RDWR)==-1)
        printf("Не удалось открыть stderr для демонизации");
    while(1)
    {}

}

int main()
{
    daemonise();
}