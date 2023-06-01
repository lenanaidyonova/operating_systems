#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
static void sigHandler(int sig) {
	printf("Catched  signal  %s\n",sig  ==  SIGUSR1  ?  "SIGUSR1"  : "SIGUSR2");
	printf("Parent = %d\n",getppid());// востанавливаем старую диспозицию
	signal(sig,SIG_DFL);
}
int main() {
	printf("\nFather started: pid = %i,ppid = %i\n",getpid(),getppid());
	signal(SIGUSR1,sigHandler);
	signal(SIGUSR2,sigHandler);
	signal(SIGINT,SIG_DFL);
	signal(SIGCHLD,SIG_IGN);
	int forkRes = fork();
	if(forkRes == 0) {// программасын
	printf("\nSon started: pid = %i,ppid = %i\n",getpid(),getppid());//отправляемсигналыотцу
	if(kill(getppid(),SIGUSR1) != 0) {
		printf("Error while sending SIGUSR1\n");
		exit(1);
	}
	printf("Successfully sent SIGUSR1\n");
	return 0;
	}// программаотец
	wait(NULL);// ждемсигналов
	for(;;){
	pause();
}
	return 0;
}
