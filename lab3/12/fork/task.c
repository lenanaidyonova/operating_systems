#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


void sigint_handler(int sig) {
    printf("Caught SIGINT signal\n");
}

int main() {
    pid_t child_pid;
    int status;

    signal(SIGINT, sigint_handler);

    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(1);
    } else if (child_pid == 0) {
        // This is the child process
        printf("Child process:\n");
        printf("  - My PID: %d\n", getpid());
        printf("  - My parent's PID: %d\n", getppid());
        printf("  - Signal disposition for SIGINT: %p\n", signal(SIGINT, SIG_DFL));
        exit(0);
    } else {
        // This is the parent process
        printf("Parent process:\n");
        printf("  - My PID: %d\n", getpid());
        printf("  - My child's PID: %d\n", child_pid);
        printf("  - Signal disposition for SIGINT: %p\n", signal(SIGINT, SIG_IGN));
        wait(&status);
        printf("Parent process: child exited with status %d\n", status);
        exit(0);
    }
}
