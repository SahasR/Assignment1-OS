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
int* buffer1;
volatile int buffer1Size;
int buffer2;
pthread_mutex_t bufferLock;
pthread_cond_t bufferLockCondition;
pthread_cond_t writeCondition;
volatile int written = 0;
volatile int totalWritten = 0;
volatile int returnAlgo;
pthread_mutex_t newLoopLock;
pthread_cond_t newLoopCond;
volatile int nextLoop;

void cleanUpHandler(void* lock){ /*File Handler to clean*/
    pid_t x = syscall(__NR_gettid);
    printf("%d has terminated\n", x);
    pthread_mutex_unlock((pthread_mutex_t *)lock);
}

void writeBuffer(int seek, int type){ /*Write to the buffer2*/
    pthread_mutex_lock(&bufferLock);
    while (written == 1){
        pthread_cond_wait(&bufferLockCondition, &bufferLock);
    }
    buffer2 = seek;
    returnAlgo = type;
    totalWritten++;
    written = 1;
    pthread_cond_signal(&bufferLockCondition);
    pthread_mutex_unlock(&bufferLock);
}

void waitLock(){ /*Wait on the File Lock*/
    pthread_mutex_lock(&fileReadLock);
    while(fileRead == 0){ 
        pthread_cond_wait(&fileReadCondition, &fileReadLock);
    }
    printf("\n");
    pthread_mutex_unlock(&fileReadLock);
}

void waitNextLoop(){ /*Wait until the next Loop is ready*/
    pthread_mutex_lock(&newLoopLock);
    while (nextLoop == 0){
        pthread_cond_wait(&newLoopCond, &newLoopLock);
    }
    printf("\n");
    pthread_mutex_unlock(&newLoopLock);
}

/* Child Threads A to F execute each seek*/

void * childThreadA(void * tid){
    int i;
    int length;
    int oldtype;
    int * temp;
    int seek;

    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);

    for (;;){
        
        waitLock();

        temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
        for (i = 3; i < buffer1Size; i++){
            temp[i-3] = buffer1[i];
        }
        seek = firstComeFirstServe(temp, buffer1Size -3, buffer1[1]);
        
        writeBuffer(seek, 1);

        free(temp);

        waitNextLoop();
    }
    pthread_cleanup_pop(0);

}

void * childThreadB(void * tid){
    int oldtype;
    int i;
    int* temp;
    int seek;
    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);

    for (;;){
        
        waitLock();
        
        temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
        for (i = 3; i < buffer1Size; i++){
            temp[i-3] = buffer1[i];
        }
        seek = shortestSeekTimeFirst(temp, buffer1Size -3, buffer1[1], buffer1[0]);

        writeBuffer(seek, 2);

        free(temp);

        waitNextLoop();
    }
    pthread_cleanup_pop(0);
}

void * childThreadC(void * tid){
    int oldtype;
    int * temp;
    int seek;
    int i;
    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    for (;;){
         
        waitLock();
        
        temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
        for (i = 3; i < buffer1Size; i++){
            temp[i-3] = buffer1[i];
        }
        seek = scan(temp, buffer1Size -3, buffer1[1], buffer1[2], buffer1[0]);

        writeBuffer(seek, 3);

        free(temp);

        waitNextLoop();
    }
    pthread_cleanup_pop(0);
}

void * childThreadD(void * tid){
    int oldtype;
    int * temp;
    int i;
    int seek;

    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    for (;;){
        
        waitLock();

        temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
        for (i = 3; i < buffer1Size; i++){
            temp[i-3] = buffer1[i];
        }
        seek = cscan(temp, buffer1Size -3, buffer1[1], buffer1[2], buffer1[0]);

        writeBuffer(seek, 4);

        free(temp);

        waitNextLoop();
    }
    pthread_cleanup_pop(0);
}

void * childThreadE(void * tid){
    int oldtype;
    int* temp;
    int seek;
    int i;

    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    for (;;){
        
        waitLock();
        
        temp = (int* )malloc((buffer1Size - 3)*sizeof(int));
        for (i = 3; i < buffer1Size; i++){
            temp[i-3] = buffer1[i];
        }
        seek = look(temp, buffer1Size -3, buffer1[1], buffer1[2]);

        writeBuffer(seek, 5);

        free(temp);

        waitNextLoop();
    }
    pthread_cleanup_pop(0);
}

void * childThreadF(void * tid){
    int oldtype;
    int* temp;
    int seek;
    int i;
    pthread_cleanup_push(cleanUpHandler, &fileReadLock);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    for (;;){
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

        writeBuffer(seek, 6);

        free(temp);

        waitNextLoop();
    }
    pthread_cleanup_pop(0);
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
    /*Initialize the Mutex's and Conditions*/
    pthread_mutex_init(&fileReadLock, NULL);
    pthread_cond_init(&fileReadCondition, NULL);
    pthread_mutex_init(&bufferLock, NULL);
    pthread_cond_init(&bufferLockCondition, NULL);
    pthread_mutex_init(&newLoopLock, NULL);
    pthread_cond_init(&newLoopCond, NULL);

    pthread_create(&tA, NULL, childThreadA, (void *)&tA);
    pthread_create(&tB, NULL, childThreadB, (void *)&tB);
    pthread_create(&tC, NULL, childThreadC, (void *)&tC);
    pthread_create(&tD, NULL, childThreadD, (void *)&tD);
    pthread_create(&tE, NULL, childThreadE, (void *)&tE);
    pthread_create(&tF, NULL, childThreadF, (void *)&tF);
    sleep(2);

    do {
        printf("Disk Schedular Simulation: ");
        scanf("%s", input);

        if (strcmp(input, "QUIT") != 0){
            status = readFile(input, &totalCylinders, &currPosition, &prevPosition, &data);
            if (status == 1){
                a = convert(data, &length);
                buffer1 = (int* )malloc((length+3)*sizeof(int));
                buffer1[0] = totalCylinders;
                buffer1[1] = currPosition;
                buffer1[2] = prevPosition;
                for (i = 0; i < length; i++){
                    buffer1[i+3] = a[i];
                }
                buffer1Size = length + 3;
                pthread_mutex_lock(&newLoopLock);
                nextLoop = 0;
                pthread_cond_broadcast(&newLoopCond);
                pthread_mutex_unlock(&newLoopLock);

                pthread_mutex_lock(&fileReadLock);
                fileRead = 1;
                pthread_cond_broadcast(&fileReadCondition);
                pthread_mutex_unlock(&fileReadLock);

                while (totalWritten != 6){
                    pthread_mutex_lock(&bufferLock);
                    while (written == 0){
                    pthread_cond_wait(&bufferLockCondition, &bufferLock);
                    }
                
                    switch (returnAlgo){
                        case 1:
                            fcfs = buffer2;
                            break;
                        case 2:
                            sstf = buffer2;
                            break;
                        case 3:
                            sc = buffer2;
                            break;
                        case 4:
                            csc = buffer2;
                            break;
                        case 5:
                            lk = buffer2;
                            break;
                        case 6:
                            clk = buffer2;
                            break;
                    }
                    
                    written = 0;
                    pthread_cond_signal(&bufferLockCondition);
                    pthread_mutex_unlock(&bufferLock);
                    sleep(0.5);
                }

                printf("For %s:\n", input);
                printf("FCFS: %d\n", fcfs);
                printf("SSTF: %d\n", sstf);
                printf("SCAN: %d\n", sc);
                printf("CSCAN: %d\n", csc);
                printf("LOOK: %d\n", lk);
                printf("CLOOK: %d\n", clk);

                written = 0;
                totalWritten = 0;
                fileRead = 0;

                pthread_mutex_lock(&newLoopLock);
                nextLoop = 1;
                pthread_cond_broadcast(&newLoopCond);
                pthread_mutex_unlock(&newLoopLock);
            }
        } else {
            pthread_cancel(tA);
            pthread_cancel(tB);
            pthread_cancel(tC);
            pthread_cancel(tD);
            pthread_cancel(tE);
            pthread_cancel(tF);
        }
        sleep(2);
    } while (strcmp(input, "QUIT") != 0);

    free(buffer1);

    return 0;
}
