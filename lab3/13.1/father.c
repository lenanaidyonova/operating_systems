#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
    int sid, pid, pid1, ppid, status;
    char command[50];
    if (argc < 2) 
        return -1;
    pid = getpid();
    ppid = getppid();
    sid = getsid(pid);
    sprintf(command, "ps -xjf | grep \"STAT\\|%d\" > %s", sid, argv[1]);
    printf("FATHER PARAMS: sid = %i  pid=%i  ppid=%i \n", sid, pid,ppid);
    if((pid1=fork())==0) 
        execl("son1","son1", NULL);

    if(fork()==0)
        execl("son2","son2", argv[1], NULL);

    if(fork()==0) execl("son3","son3", NULL);
    system(command);
   
    waitpid(pid1, &status, WNOHANG); 
}
