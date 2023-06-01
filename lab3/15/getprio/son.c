#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>

int main(int argc, char *argv[])
{
    int pid, ppid;

    pid = getpid();
    ppid = getppid();

    int pr = getpriority(PRIO_PROCESS, pid);
    printf("son params: pid = %i ppid = %i priority = %i\n",pid,ppid,pr);

    sleep(2);

    return 0;
}
