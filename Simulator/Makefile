##
 # File: Makefile
 # Author: Sahas Gunasekara
 # -----
 # Modified By: Sahas Gunasekara
 # -----
 # Purpose: Makefile for OS Assignment
##

# Makefile Variables
CC = gcc -pthread
EXEC = simulator
CFLAGS = -g

$(EXEC) : simulator.o scheduler.o
	$(CC) simulator.o scheduler.o -o $(EXEC) $(CFLAGS)

scheduler.o : scheduler.c scheduler.h
	$(CC) -c scheduler.c $(CFLAGS)

simulator.o : simulator.c
	$(CC) -c simulator.c $(CFLAGS)

clean:
	rm -f $(EXEC) simulator.o scheduler.o