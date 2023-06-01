#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig_no) {
    if (sig_no == SIGTSTP) {
        puts("^Z - signal received");
        signal(SIGTSTP, SIG_DFL); // restore default disposition
    }
}

int main() {
	int pid, ppid;
	pid = getpid();
	ppid = getppid();
	printf("Current pid = %d and ppid = %d\n", pid, ppid);
    signal(SIGTSTP, handler);
    while(1) {
        // do something here
    }
    return 0;
}

