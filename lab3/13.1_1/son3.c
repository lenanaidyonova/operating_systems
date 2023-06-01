#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


void SIGUSR1_handler(int sig_no)
{
    printf("SIGUSR1_handler running!\n");
}


int main() {
    
    signal(SIGUSR1, SIGUSR1_handler);
	sleep(10);
}
