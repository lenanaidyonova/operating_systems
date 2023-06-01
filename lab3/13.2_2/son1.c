#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void SIGUSR1_handler(int sig_no)
{
    time_t st_t;
    st_t = time(NULL);
    
    printf("son1 (sleeping) received the signal at time %s\n", ctime(&st_t));
    
    exit(0);
}

int main() {
    signal(SIGUSR1, SIGUSR1_handler);
    printf("son1 running\n");
    
    sleep(3);

    return 0;
}
