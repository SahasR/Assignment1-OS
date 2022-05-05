##
 # File: Makefile
 # Author: Sahas Gunasekara
 # -----
 # Modified By: Sahas Gunasekara
 # -----
 # Purpose: Makefile for OS Assignment
##

# Makefile Variables
CC = gcc
EXEC = scheduler
CFLAGS = -Wall -ansi -pedantic -Werror -g

$(EXEC) : scheduler.o 
	$(CC) scheduler.c -o $(EXEC) $(CFLAGS)

clean:
	rm -f $(EXEC) scheduler.o