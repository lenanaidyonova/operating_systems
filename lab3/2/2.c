#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



int main() {
    int pid;
    int n = 100;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    while(1) {
        if (pid == 0) {
            printf("new pid = %d, ppid = %d\n", getpid(),getppid() ); 
            /*здесь размещаются вычисления, выполняемые процессом-потомком */
            printf("Текущее значение %d\n", n);
            n += 2;
        } else {
            printf("parent pid = %d, ppid = %d\n", getpid(),getppid() ); 
            /*здесь размещаются вычисления, выполняемые порождающим процессом */
            printf("Текущее значение %d\n", n);
            n -= 3;
        }
    }
    printf("Завершение процесса\n");
    exit(1);

    return 0;
}
