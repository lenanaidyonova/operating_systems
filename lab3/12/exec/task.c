#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

void handler(int signum) {
    printf("Received signal %d in PID %d\n", signum, getpid());
}

int main() {
    int fd = open("test.txt", O_CREAT | O_WRONLY, 0666);

    // Set a signal handler on the file descriptor
    signal(SIGUSR1, handler);
    fcntl(fd, F_SETOWN, getpid());

    pid_t pid = fork();

    if (pid == 0) {
        // In the child process, inherit the file descriptor from the parent
        // and print the current signal disposition
        printf("Child PID %d inherited file descriptor %d\n", getpid(), fd);
        printf("Current signal disposition: %p\n", signal(SIGUSR1, SIG_DFL));

        // Execute a new program image using the 'ls' command
        execlp("ls", "ls", "-l", NULL);
    } else if (pid > 0) {
        // In the parent process, wait for the child to exit
        wait(NULL);
    }

    return 0;
}
