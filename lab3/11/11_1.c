#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
    struct sched_param shdprm;
    struct timespec qp;
    int i, pid, pid1, pid2, pid3, ppid, status;

    pid = getpid();
    ppid = getppid();
    printf("FATHER PARAMS: pid=%i  ppid=%i\n", pid, ppid);

    if (nice(1000) == -1)
        perror("NICE");
    else
        printf("Nice value = %d\n", nice(1000));

    shdprm.sched_priority = 50;
    if (sched_setscheduler(pid, SCHED_RR, &shdprm) == -1)
        perror("SCHED_SETSCHEDULER_1");

    if (sched_rr_get_interval(pid, &qp) == -1)
        perror("SCHED_RR_GET_INTERVAL");
    else
        printf("Квант при циклическом планировании: %ld сек %ld нс\n", qp.tv_sec, qp.tv_nsec);

    pid1 = fork();
    if (pid1 == 0) {
        if (sched_rr_get_interval(pid1, &qp) == -1)
            perror("SCHED_RR_GET_INTERVAL");
        else
            printf("SON: Квант процессорного времени: %ld сек %ld нс\n", qp.tv_sec, qp.tv_nsec);

        execl("./son", "son", NULL);
        exit(EXIT_FAILURE);
    }

    printf("Процесс с pid = %d завершен\n", wait(&status));
    return 0;
}
