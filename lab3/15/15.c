#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>

int main() {
    int pr, pid;
    pid = getpid();
    pr = getpriority(PRIO_PROCESS, pid);

    printf("Priority is %d\n", pr);
    //nice(1000);
    setpriority(PRIO_PROCESS, pid, pr + 1); // increase priority by 1

    pr = getpriority(PRIO_PROCESS, pid);
    printf("New priority is %d\n", pr);

    return 0;
}
