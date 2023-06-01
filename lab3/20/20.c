#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_t t1, t2;

void* thread1(void* arg) {
    int count = 0;
    int i;
    printf("Thread 1 started\n");
    for (i = 0; i < 2; i++) {
        count++;
        sleep(5);
        printf("Thread 1: slept %d times\n", count);
        pthread_kill(t2, SIGUSR1);
    }
    pthread_exit(NULL);
}

void* thread2(void* arg) {
    int count = 0;
    int i;
    printf("Thread 2 started\n");
    system("ps -T");
    for (i = 0; i < 10; i++) {
        count++;
        sleep(1);
        printf("Thread 2: slept %d times\n", count);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    
    return 0;
}
