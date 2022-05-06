/*Header file for the Scheduler.c file functions*/

#ifndef SCHEDULAR_H
#define SCHEDULAR_H

void readFile(char* input, int* totalCylinders, int* currPosition, int* prevPosition, char** data);

int* convert(char* c, int* length);

int firstComeFirstServe(int* a, int length, int currposition);

int shortestSeekTimeFirst(int* a, int length, int currposition, int totalcylinders);

int scan(int* a, int length, int currPosition, int prevPosition, int totalCylinders);

int cscan(int* a, int length, int currPosition, int prevPosition, int totalCyliners);

int look(int* a, int length, int currPosition, int prevPosition);

int clook(int* a, int length, int currPosition, int prevPosition);

#endif

