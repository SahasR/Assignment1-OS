#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "scheduler.h"

pthread_mutex_t fileReadLock;
pthread_cond_t fileReadCondition;
volatile int fileRead = 0;

void * childThread(void * tid){
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        printf("Thread is blocked!");
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    pthread_mutex_unlock(&fileReadLock);
    printf("Inside wohoo!");
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

    pthread_t tidA;
    pthread_t tidB;
    pthread_t tidC;
    pthread_t tidD;
    pthread_t tidE;
    pthread_t tidF;

    pthread_mutex_init(&fileReadLock, NULL);
    pthread_cond_init(&fileReadCondition, NULL);

    pthread_create(&tidA, NULL, childThread, (void *)&tidA);
    pthread_create(&tidB, NULL, childThread, (void *)&tidB);
    pthread_create(&tidC, NULL, childThread, (void *)&tidC);
    pthread_create(&tidD, NULL, childThread, (void *)&tidD);
    pthread_create(&tidE, NULL, childThread, (void *)&tidE);
    pthread_create(&tidF, NULL, childThread, (void *)&tidF);

    printf("Disk Schedular Simulation: ");
    scanf("%s", input);
    
    status = readFile(input, &totalCylinders, &currPosition, &prevPosition, &data);
    a = convert(data, &length);
    pthread_mutex_lock(&fileReadLock);
    fileRead = 1;
    pthread_cond_signal(&fileReadCondition);
    pthread_mutex_unlock(&fileReadLock);

    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);
    pthread_join(tidC, NULL);
    pthread_join(tidD, NULL);
    pthread_join(tidE, NULL);
    pthread_join(tidF, NULL);
    
    pthread_mutex_destroy(&fileReadLock);
    pthread_cond_destroy(&fileReadCondition);

    return 0;
}
