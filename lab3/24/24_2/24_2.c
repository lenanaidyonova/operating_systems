#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#define BILLION 1000000000.0

const int COUNT = 150000000;
const int COUNT_THREADS = 6;
const int LENGHT_FILE_LINE = 10;

void printFirstPS()
{
    char command[80];
    sprintf(command, "ps -T -p %d -o f,s,pid,ppid,spid,cls,pri,ni,stat,cmd,rtprio > First_threads.txt", getpid());
    system(command);
}

void printSecondPS()
{
    char command[80];
    sprintf(command, "ps -T -p %d -o f,s,pid,ppid,spid,cls,pri,ni,stat,cmd,rtprio > Second_threads.txt", getpid());
    system(command);
}

void *Thread_func(void *arg) {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    int *id = (int *) arg;
    int tid = syscall(SYS_gettid);
    int pid = getpid();
    int a = 0;

    if(*id % 10 == 1)
        printFirstPS();
    else
        printSecondPS();

    printf("\nПоток№%d c tid = %d b pid = %d начал работу\n",
           *id, tid, pid);
    for (int i = 0; i < COUNT; ++i) {
        a++;
    }
    clock_gettime(CLOCK_REALTIME, &end);
    
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / BILLION;

    printf("\nПоток№%d c id = %d b pid = %d закончил работу, %fs\n",
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
            if(i < 6)
            {
                priors[i] = atoi(line);
            } else if(i < 12) {
                policies[i - 6] = atoi(line);
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
    pthread_t first_process_threads[COUNT_THREADS];
    pthread_attr_t first_process_thread_attributes[COUNT_THREADS];

    pthread_t second_process_threads[COUNT_THREADS];
    pthread_attr_t second_process_thread_attributes[COUNT_THREADS];

    int priorities[COUNT_THREADS], policies[COUNT_THREADS];
    int inherit = 0;
    int first_IDs[COUNT_THREADS];
    int second_IDs[COUNT_THREADS];

    int pid;
    for(int i = 0; i < COUNT_THREADS; i++)
    {
        first_IDs[i] = i + 10;
        second_IDs[i] = i + 20;
    }

    int policy;
    struct sched_param param;

    for(int i = 0; i < COUNT_THREADS; i++)                      // инициализируем аттрибуты
    {
        pthread_attr_init(&first_process_thread_attributes[i]);
        pthread_attr_init(&second_process_thread_attributes[i]);
    }

    read_from_file(priorities, policies, &inherit);

    for(int i = 0; i < COUNT_THREADS; i++)
    {
        pthread_attr_setschedpolicy(&first_process_thread_attributes[i], policies[i]);
        pthread_attr_setschedpolicy(&second_process_thread_attributes[i], policies[i]);
        param.sched_priority = priorities[i];
        pthread_attr_setschedparam(&first_process_thread_attributes[i], &param);
        pthread_attr_setschedparam(&second_process_thread_attributes[i], &param);
    }

    if(inherit)
    {
        for(int i = 0; i < COUNT_THREADS; i++)
        {
            pthread_attr_setdetachstate(&first_process_thread_attributes[i], PTHREAD_INHERIT_SCHED);
            pthread_attr_setdetachstate(&second_process_thread_attributes[i], PTHREAD_INHERIT_SCHED);
        }
    }
    else
    {
        for(int i = 0; i < COUNT_THREADS; i++)
        {
            pthread_attr_setinheritsched(&first_process_thread_attributes[i], PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setinheritsched(&second_process_thread_attributes[i], PTHREAD_EXPLICIT_SCHED);
        }
    }

    /**
     * Вывод параметров политики потоков для первого процесса
     */
    printf("Первый процесс\n");
    for(int i = 0; i < COUNT_THREADS; i++)
    {
        pthread_attr_getschedparam(&first_process_thread_attributes[i], &param);
        pthread_attr_getschedpolicy(&first_process_thread_attributes[i], &policy);
        printf("Поток№%d, его приоритет = %d\n", i + 1, param.sched_priority);
    }

    /**
     * Вывод параметров политики потоков для второго процесса
     */
    printf("Второй процесс\n");
    for(int i = 0; i < COUNT_THREADS; i++)
    {
        pthread_attr_getschedparam(&second_process_thread_attributes[i], &param);
        pthread_attr_getschedpolicy(&second_process_thread_attributes[i], &policy);
        printf("Поток№%d, его приоритет = %d\n", i + 1, param.sched_priority);
    }

    pid = fork();

    switch (pid) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            for(int i = 0; i < COUNT_THREADS; i++)
            {
                if(pthread_create(&second_process_threads[i], &second_process_thread_attributes[i],
                                  Thread_func, &second_IDs[i]))
                    perror("Статус создания потока процесса-потомка");
            }
            for(int i = 0; i < COUNT_THREADS; i++)
                pthread_join(second_process_threads[i], NULL);
            for(int i = 0; i < COUNT_THREADS; i++)
                pthread_attr_destroy(&second_process_thread_attributes[i]);
            exit(EXIT_SUCCESS);
        default:
            for(int i = 0; i < COUNT_THREADS; i++)
            {
                if(pthread_create(&first_process_threads[i], &first_process_thread_attributes[i],
                                  Thread_func, &first_IDs[i]))
                    perror("Статус создания потока процесса-родителя");
            }
            system("ps -o f,s,pid,ppid,cls,pri,ni,stat,cmd,rtprio xf > proc.txt");
            for(int i = 0; i < COUNT_THREADS; i++)
                pthread_join(first_process_threads[i], NULL);
            for(int i = 0; i < COUNT_THREADS; i++)
                pthread_attr_destroy(&first_process_thread_attributes[i]);
            exit(EXIT_SUCCESS);
    }
}
