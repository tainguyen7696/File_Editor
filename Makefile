########################################################
# Name: Tai Nguyen, Ryan Stobbe, Branden Wallace
# Account: cssc1218
# Class: CS570, Summer 2018
# Assignment Info: Assignment #2 File Management
# Filename: Makefile
#########################################################

CC=gcc
FILES=menu.c process.h
NEWEXEC=filem
OLDEXEC=a.out
DEL=process.h.gch filem
all:
	$(CC) $(FILES)
	mv $(OLDEXEC) $(NEWEXEC)
	rm -f *.o

clean:
	rm $(DEL)
##################[ EOF: Makefile ]#######################
