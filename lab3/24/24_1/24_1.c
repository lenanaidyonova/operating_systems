#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#define BILLION  1000000000.0

typedef struct Data Data;
const int COUNT = 1500000;
const int COUNT_THREADS = 18;
const int LENGHT_FILE_LINE = 10;

void printPS()
{
    char command[80];
    sprintf(command, "ps -T -p %d -o f,s,pid,ppid,spid,cls,pri,ni,stat,cmd,rtprio > file.txt", getpid());
    system(command);
}

void *Thread_func(void *arg) {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    int *id = (int *) arg;
    int tid = syscall(SYS_gettid);
    int pid = getpid();
    int a = 0;
    printPS();
    //system( "ps -T > file.txt");
    printf("\nThread_%d with tid = %d and pid = %d is started\n",
           *id, tid, pid);
    for (int i = 0; i < COUNT; ++i) {
        a++;
        // if(a % 100 == 0)
        //     printf("Thread_%d - %d\n", *id, a);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    // time_spent = конец - начало
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / BILLION;

    printf("\nThread_%d with id = %d and pid = %d is completed, %fs\n",
           *id, tid, pid, time_spent);
}


int read_from_file(int *priors, int *policies, int *isInherits)
{
    FILE *file = fopen("params.txt", "r");
    char line[LENGHT_FILE_LINE];
    if(file)
    {
        int i = 0;
        while(fgets(line, LENGHT_FILE_LINE, file) != NULL)
        {
            if(i < 18)
            {
                priors[i] = atoi(line);
            } else if(i < 36) {
                policies[i - 18] = atoi(line);
            } else
                *isInherits = atoi(line);
            i++;
        }
    }
    else
        return -1;
    fclose(file);
    return 0;
}

int main()
{
    pthread_t threads[COUNT_THREADS];
    pthread_attr_t thread_attributes[COUNT_THREADS];
    int priorities[COUNT_THREADS], policies[COUNT_THREADS];
    int inherit = 0;
    int IDs[COUNT_THREADS];
    for(int i = 0; i < COUNT_THREADS; i++)
        IDs[i] = i + 1;

    int policy;
    struct sched_param param;

    for(int i = 0; i < COUNT_THREADS; i++)                      // инициализируем
        pthread_attr_init(&thread_attributes[i]);           // описателей атрибутов

    read_from_file(priorities, policies, &inherit);

    for(int i = 0; i < COUNT_THREADS; i++)
    {
        pthread_attr_setschedpolicy(&thread_attributes[i], policies[i]);
        param.sched_priority = priorities[i];
        pthread_attr_setschedparam(&thread_attributes[i], &param);
    }

    if(inherit == 1)
    {
        for(int i = 0; i < COUNT_THREADS; i++)
            pthread_attr_setdetachstate(&thread_attributes[i], PTHREAD_INHERIT_SCHED);
    }
    else
    {
        for(int i = 0; i < COUNT_THREADS; i++)
            pthread_attr_setinheritsched(&thread_attributes[i], PTHREAD_EXPLICIT_SCHED);
    }

    for(int i = 0; i < COUNT_THREADS; i++)
    {
        pthread_attr_getschedparam(&thread_attributes[i], &param);
        pthread_attr_getschedpolicy(&thread_attributes[i], &policy);
        printf("Поток№%d, его приоритет = %d\n", i + 1, param.sched_priority);
    }

    switch (policy) {
        case SCHED_FIFO:
            printf ("Политика процесса: SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf ("Политика процесса: SCHED_RR\n");
            break;
        case SCHED_OTHER:
            printf ("Политика процесса: SCHED_OTHER\n");
            break;
        case -1:
            perror ("Политика процесса: SCHED_GETSCHEDULER");
            break;
        default:
            printf ("Политика процесса: Неизвестная политика планирования\n");
    }
	
    for(int i = 0; i < COUNT_THREADS; i++)
    {
        if(pthread_create(&threads[i], &thread_attributes[i], Thread_func, &IDs[i]))
            perror("Статус создания потока");
    }
   
    for(int i = 0; i < COUNT_THREADS; i++)
        pthread_join(threads[i], NULL);
    // system("ps -T > file.txt");
    for(int i = 0; i < COUNT_THREADS; i++)
        pthread_attr_destroy(&thread_attributes[i]);
    
    return 0;
}
