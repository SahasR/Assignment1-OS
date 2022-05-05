#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

void readFile(char* input, int* totalCylinders, int* currPosition, int* prevPosition, char** data){
    char str[1000];
    char* token;
    FILE *in_file = fopen(input, "r");

    if (in_file == NULL){
        printf("Couldn't open the file!\n");
        exit(-1);
    }
    else {
       fgets(str, 1000, in_file);
       token = strtok(str, " ");
       *totalCylinders = atoi(token);
       token = strtok(NULL, " ");
       *currPosition = atoi(token);
       token = strtok(NULL, " ");
       *prevPosition = atoi(token);
       token = strtok(NULL, "");
       *data = token;
    }
}

int* convert(char* c, int* length){
    char dest[1000];
    char* token;
    int* a;
    int space = 0;
    int i = 1;

    strcpy(dest, c);

    while (i <= dest[i]){
        if (dest[i] == ' '){
            space++;
        }
        i++;
    }

    *length = space + 1;

    a = (int* )malloc((space+1)*sizeof(int));
    
    token = strtok(dest, " ");
    a[0] = atoi(token);
    
    for (i = 1; i <= space; i++){
        token = strtok(NULL, " ");
        a[i] = atoi(token);
    }

    return a;
}

int firstComeFirstServe(int* a, int length, int currposition){
    int i;
    int seek = 0;
    int diff;
    diff = a[0] - currposition;
    seek += abs(diff);

    for (i = 0; i < length - 1; i++){
        diff = a[i+1] - a[i];
        seek += abs(diff);
    }

    return seek;
}

int shortestSeekTimeFirst(int* a, int length, int currposition, int totalcylinders){
    int i;
    int currTotal = 0;
    int seek = 0;
    int minumSeek = totalcylinders;
    int diff;
    int currIndex;
    int newPosition;

    int* tempSeek;
    int* tempArray;

    tempArray = (int* )malloc((length)*sizeof(int));
    tempSeek = (int* )malloc((length)*sizeof(int));

    for (i = 0; i < length; i++){
        tempArray[i] = a[i]; 
    }

    for (i = 0; i < length; i++){
        diff = tempArray[i] - currposition;
        tempSeek[i] = abs(diff);
    }

    do {
        for (i = 0; i < length; i++){
            if (minumSeek > tempSeek[i]){
                minumSeek = tempSeek[i];
                currIndex = i;
                newPosition = tempArray[i];
            }
        }

        seek += minumSeek;
        currTotal++;

        tempArray[currIndex] = totalcylinders;

        for (i = 0; i < length; i++){
            diff = tempArray[i] - newPosition;
            tempSeek[i] = abs(diff);
        }   

        minumSeek = totalcylinders;

    } while (currTotal != length);

    free(tempArray);
    free(tempSeek);
    return seek;
}

int scan(int* a, int length, int currPosition, int prevPosition, int totalCylinders){
    int i;
    int j;
    int temp;
    int r2l = 0;
    int* tempArray;
    int* tempOrder;
    int pivot;
    int diff;
    int seek = 0;

    tempArray = (int* )malloc((length+1)*sizeof(int));
    tempOrder = (int* )malloc((length+1)*sizeof(int));

    for (i = 0; i < length; i++){
        tempArray[i] = a[i];
    }

    for (i = 0; i < length; i++){
        for (j = i+1; j < length; j++){
            if (tempArray[i] > tempArray[j]){
                temp = tempArray[i];
                tempArray[i] = tempArray[j];
                tempArray[j] = temp;
            }
        }
    }

    if (currPosition < prevPosition){
        r2l = 1;
    }

    if (r2l == 1){
        for (i = 0; i < length; i++){
            if (tempArray[i] < currPosition){
                pivot = i;
            }
        } 

        for (i = 0; i <= pivot; i++){
            for (j = i+1; j <= pivot; j++){
                if (tempArray[i] < tempArray[j]){
                temp = tempArray[i];
                tempArray[i] = tempArray[j];
                tempArray[j] = temp;
                }   
            }
        }

        for (i = length; i > pivot; i--){
            tempArray[i] = tempArray[i-1]; 
        }

        tempArray[pivot+1] = 0;

        diff =  tempArray[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length; i++){
            diff = tempArray[i+1] - tempArray[i];
            seek += abs(diff); 
        }

    } else {
        for (i = 0; i < length; i++){
            if (tempArray[i] < currPosition){
                pivot = i;
            }
        } 

        for (i = pivot+1; i < length; i++){
            tempOrder[i-(pivot+1)] = tempArray[i];
        }

        tempOrder[length - (pivot+1)] = totalCylinders - 1;

        for (i = 0; i <= pivot; i++){
            tempOrder[length - i] = tempArray[i];
        }

        diff =  tempOrder[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length; i++){
            diff = tempOrder[i+1] - tempOrder[i];
            seek += abs(diff); 
        }

    }   

    free(tempArray);
    free(tempOrder);
    return seek;
}

int cscan(int* a, int length, int currPosition, int prevPosition, int totalCyliners){
    int* tempArray;
    int* tempOrder;
    int i;
    int temp;
    int j;
    int r2l = 0;
    int pivot;
    int diff;
    int seek = 0;

    tempArray = (int* )malloc((length+2)*sizeof(int));
    tempOrder = (int* )malloc((length+2)*sizeof(int));

    for (i = 0; i < length; i++){
        tempArray[i] = a[i];
    }

    if (currPosition < prevPosition){
        r2l = 1;
    }

    for (i = 0; i < length; i++){
        for (j = i+1; j < length; j++){
            if (tempArray[i] > tempArray[j]){
                temp = tempArray[i];
                tempArray[i] = tempArray[j];
                tempArray[j] = temp;
            }
        }
    }

    for (i = 0; i < length; i++){
            if (tempArray[i] < currPosition){
                pivot = i;
            }
        }

    if (r2l == 1){

        for (i = pivot + 1; i < length; i++){
            for (j = i+1; j < length; j++){
                if (tempArray[i] < tempArray[j]){
                    temp = tempArray[i];
                    tempArray[i] = tempArray[j];
                    tempArray[j] = temp;
                }
            }
        }

        for (i = 0; i <= pivot; i++){
            for (j = i+1; j <= pivot; j++){
                if (tempArray[i] < tempArray[j]){
                    temp = tempArray[i];
                    tempArray[i] = tempArray[j];
                    tempArray[j] = temp;
                }
            }
        }

        for (i = length + 1; i > pivot; i--){
            tempArray[i] = tempArray[i-2]; 
        }

        tempArray[pivot + 1] = 0;
        tempArray[pivot + 2] = totalCyliners -1;

        diff =  tempArray[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length + 1; i++){
            diff = tempArray[i+1] - tempArray[i];
            seek += abs(diff);
        }
    } else {
        for (i = pivot + 1; i < length; i++){
            tempOrder[i - (pivot+1)] = tempArray[i];
        }

        tempOrder[length - (pivot+1)] = totalCyliners - 1;
        tempOrder[length - (pivot+1) + 1] = 0;

        for (i = 0; i <= pivot; i++){
            tempOrder[length - pivot + 1 + i] = tempArray[i];
        }

        diff =  tempOrder[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length + 1; i++){
            diff = tempOrder[i+1] - tempOrder[i];
            seek += abs(diff); 
        }
    }

    free(tempArray);
    free(tempOrder);
    return seek;

}

int look(int* a, int length, int currPosition, int prevPosition){
    int i;
    int j;
    int temp;
    int r2l = 0;
    int* tempArray;
    int* tempOrder;
    int pivot;
    int diff;
    int seek = 0;

    tempArray = (int* )malloc((length)*sizeof(int));
    tempOrder = (int* )malloc((length)*sizeof(int));

    for (i = 0; i < length; i++){
        tempArray[i] = a[i];
    }

    for (i = 0; i < length; i++){
        for (j = i+1; j < length; j++){
            if (tempArray[i] > tempArray[j]){
                temp = tempArray[i];
                tempArray[i] = tempArray[j];
                tempArray[j] = temp;
            }
        }
    }

    if (currPosition < prevPosition){
        r2l = 1;
    }

    if (r2l == 1){
        for (i = 0; i < length; i++){
            if (tempArray[i] < currPosition){
                pivot = i;
            }
        } 

        for (i = 0; i <= pivot; i++){
            for (j = i+1; j <= pivot; j++){
                if (tempArray[i] < tempArray[j]){
                temp = tempArray[i];
                tempArray[i] = tempArray[j];
                tempArray[j] = temp;
                }   
            }
        }

        diff =  tempArray[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length-1; i++){
            diff = tempArray[i+1] - tempArray[i];
            seek += abs(diff); 
        }

    } else {
        for (i = 0; i < length; i++){
            if (tempArray[i] < currPosition){
                pivot = i;
            }
        } 

        for (i = pivot+1; i < length; i++){
            tempOrder[i-(pivot+1)] = tempArray[i];
        }

        for (i = 0; i <= pivot; i++){
            tempOrder[length - i - 1] = tempArray[i];
        }

        diff =  tempOrder[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length - 1; i++){
            diff = tempOrder[i+1] - tempOrder[i];
            seek += abs(diff); 
        }

    }   

    free(tempArray);
    free(tempOrder);
    return seek;
}

int clook(int* a, int length, int currPosition, int prevPosition){
    int* tempArray;
    int* tempOrder;
    int i;
    int temp;
    int j;
    int r2l = 0;
    int pivot;
    int diff;
    int seek = 0;

    tempArray = (int* )malloc((length)*sizeof(int));
    tempOrder = (int* )malloc((length)*sizeof(int));

    for (i = 0; i < length; i++){
        tempArray[i] = a[i];
    }

    if (currPosition < prevPosition){
        r2l = 1;
    }

    for (i = 0; i < length; i++){
        for (j = i+1; j < length; j++){
            if (tempArray[i] > tempArray[j]){
                temp = tempArray[i];
                tempArray[i] = tempArray[j];
                tempArray[j] = temp;
            }
        }
    }

    for (i = 0; i < length; i++){
            if (tempArray[i] < currPosition){
                pivot = i;
            }
        }

    if (r2l == 1){

        for (i = pivot + 1; i < length; i++){
            for (j = i+1; j < length; j++){
                if (tempArray[i] < tempArray[j]){
                    temp = tempArray[i];
                    tempArray[i] = tempArray[j];
                    tempArray[j] = temp;
                }
            }
        }

        for (i = 0; i <= pivot; i++){
            for (j = i+1; j <= pivot; j++){
                if (tempArray[i] < tempArray[j]){
                    temp = tempArray[i];
                    tempArray[i] = tempArray[j];
                    tempArray[j] = temp;
                }
            }
        }

        diff =  tempArray[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length - 1; i++){
            diff = tempArray[i+1] - tempArray[i];
            seek += abs(diff);
        }
    } else {
        for (i = pivot + 1; i < length; i++){
            tempOrder[i - (pivot+1)] = tempArray[i];
        }

        for (i = 0; i <= pivot; i++){
            tempOrder[length - pivot - 1 + i] = tempArray[i];
        }

        diff =  tempOrder[0] - currPosition;
        seek += abs(diff);

        for (i = 0; i < length - 1; i++){
            diff = tempOrder[i+1] - tempOrder[i];
            seek += abs(diff); 
        }
    }

    free(tempArray);
    free(tempOrder);
    return seek;
}

int main(int argc, char* argv[])
{
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

    do {
        printf("Disk Schedular Simulation: ");
        scanf("%s", input); 

        if (strcmp(input, "QUIT") != 0){
            readFile(input, &totalCylinders, &currPosition, &prevPosition, &data);
            a = convert(data, &length);
            fcfs = firstComeFirstServe(a, length, currPosition);
            sstf = shortestSeekTimeFirst(a, length, currPosition, totalCylinders);
            sc = scan(a, length, currPosition, prevPosition, totalCylinders);
            csc = cscan(a, length, currPosition, prevPosition, totalCylinders);
            lk = look(a, length, currPosition, prevPosition);
            clk = clook(a, length, currPosition, prevPosition);

            printf("For %s:\n", input);
            printf("FCFS: %d\n", fcfs);
            printf("SSTF: %d\n", sstf);
            printf("SCAN: %d\n", sc);
            printf("CSCAN: %d\n", csc);
            printf("LOOK: %d\n", lk);
            printf("CLOOK: %d\n", clk);
        }
    } while (strcmp(input, "QUIT") != 0);

    free(a);
    
    return 0;
}