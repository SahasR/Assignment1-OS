#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "scheduler.h"

pthread_mutex_t fileReadLock;
pthread_cond_t fileReadCondition;
volatile int fileRead = 0;
volatile int* buffer1;
volatile int buffer1Size;
volatile int* buffer2;

void cleanUpHandler(void* lock){
    pid_t x = syscall(__NR_gettid);
    printf("%d has terminated\n", x);
    pthread_mutex_unlock((pthread_mutex_t *)lock);
}

void * childThreadA(void * tid){
    int i;
    int length;
    int oldtype;
    int * temp;
    int seek;
    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    pthread_mutex_unlock(&fileReadLock);
    
    temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
    for (i = 3; i < buffer1Size; i++){
        temp[i-3] = buffer1[i];
    }
    seek = firstComeFirstServe(temp, buffer1Size -3, buffer1[1]);
    printf("FCFS : %d\n", seek);
    free(temp);
    
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

void * childThreadB(void * tid){
    int oldtype;
    int i;
    int* temp;
    int seek;
    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    pthread_mutex_unlock(&fileReadLock);
    
    temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
    for (i = 3; i < buffer1Size; i++){
        temp[i-3] = buffer1[i];
    }
    seek = shortestSeekTimeFirst(temp, buffer1Size -3, buffer1[1], buffer1[0]);
    printf("SSTF : %d\n", seek);
    free(temp);
    
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

void * childThreadC(void * tid){
    int oldtype;
    int * temp;
    int seek;
    int i;
    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    pthread_mutex_unlock(&fileReadLock);
    
    temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
    for (i = 3; i < buffer1Size; i++){
        temp[i-3] = buffer1[i];
    }
    seek = scan(temp, buffer1Size -3, buffer1[1], buffer1[2], buffer1[0]);
    printf("SCAN : %d\n", seek);
    free(temp);
    
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

void * childThreadD(void * tid){
    int oldtype;
    int * temp;
    int i;
    int seek;

    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    pthread_mutex_unlock(&fileReadLock);

    temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
    for (i = 3; i < buffer1Size; i++){
        temp[i-3] = buffer1[i];
    }
    seek = cscan(temp, buffer1Size -3, buffer1[1], buffer1[2], buffer1[0]);
    printf("C-SCAN : %d\n", seek);
    free(temp);
    
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

void * childThreadE(void * tid){
    int oldtype;
    int* temp;
    int seek;
    int i;

    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    pthread_mutex_unlock(&fileReadLock);
    
    temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
    for (i = 3; i < buffer1Size; i++){
        temp[i-3] = buffer1[i];
    }
    seek = look(temp, buffer1Size -3, buffer1[1], buffer1[2]);
    printf("LOOK : %d\n", seek);
    free(temp);

    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

void * childThreadF(void * tid){
    int oldtype;
    int* temp;
    int seek;
    int i;
    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    pthread_mutex_unlock(&fileReadLock);

    temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
    for (i = 3; i < buffer1Size; i++){
        temp[i-3] = buffer1[i];
    }
    seek = clook(temp, buffer1Size -3, buffer1[1], buffer1[2]);
    printf("C-LOOK : %d\n", seek);
    free(temp);
    
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int totalCylinders;
    int length;
    int currPosition;
    int prevPosition;
    int fcfs;
    int sstf;
    int sc;
    int csc;
    int lk;
    int clk;
    char* data = NULL;
    int* a;
    char input[10];
    int status;
    int i;

    pthread_t tA;
    pthread_t tB;
    pthread_t tC;
    pthread_t tD;
    pthread_t tE;
    pthread_t tF;

    do {
        pthread_mutex_init(&fileReadLock, NULL);
        pthread_cond_init(&fileReadCondition, NULL);

        pthread_create(&tA, NULL, childThreadA, (void *)&tA);
        pthread_create(&tB, NULL, childThreadB, (void *)&tB);
        pthread_create(&tC, NULL, childThreadC, (void *)&tC);
        pthread_create(&tD, NULL, childThreadD, (void *)&tD);
        pthread_create(&tE, NULL, childThreadE, (void *)&tE);
        pthread_create(&tF, NULL, childThreadF, (void *)&tF);
        sleep(2);

        printf("Disk Schedular Simulation: ");
        scanf("%s", input);

        if (strcmp(input, "QUIT") != 0){
            status = readFile(input, &totalCylinders, &currPosition, &prevPosition, &data);
            if (status == 1){
                a = convert(data, &length);
                buffer1 = (int* )malloc((length+3)*sizeof(int));
                buffer2 = (int* )malloc(6*sizeof(int));
                buffer1[0] = totalCylinders;
                buffer1[1] = currPosition;
                buffer1[2] = prevPosition;
                for (i = 0; i < length; i++){
                    buffer1[i+3] = a[i];
                }
                buffer1Size = length + 3;
                pthread_mutex_lock(&fileReadLock);
                fileRead = 1;
                pthread_cond_signal(&fileReadCondition);
                pthread_cond_broadcast(&fileReadCondition);
                pthread_mutex_unlock(&fileReadLock);
            } else {
                printf("File not found so cancelling threads!\n");
                pthread_cancel(tA);
                pthread_cancel(tB);
                pthread_cancel(tC);
                pthread_cancel(tD);
                pthread_cancel(tE);
                pthread_cancel(tF);
            }
        } else {
            pthread_cancel(tA);
            pthread_cancel(tB);
            pthread_cancel(tC);
            pthread_cancel(tD);
            pthread_cancel(tE);
            pthread_cancel(tF);
        }
        pthread_join(tA, NULL);
        pthread_join(tB, NULL);
        pthread_join(tC, NULL);
        pthread_join(tD, NULL);
        pthread_join(tE, NULL);
        pthread_join(tF, NULL);

        pthread_mutex_destroy(&fileReadLock);
        pthread_cond_destroy(&fileReadCondition);
        fileRead = 0;
    } while (strcmp(input, "QUIT") != 0);

    
    
    

    return 0;
}
