
CFLAGS= -Wall
CFLAGS+=-I./include
CSTFLAGS =-DDEBUG 


LIBS=-lpthread

ifeq ($(PC), 1)
	CFLAGS +=-DPC
	CFLAGS +=-DDEBUG
endif



###########################
#		   MAIN			  #
###########################




###########################
#		   SERVEUR		  #
###########################

bin/serveur: src/serveur.c
	gcc -g $(CFLAGS) $^ -o $@ $(LIBS)






###########################
#		 CLEAN			  #
###########################	

clean: 
	@rm -f ./bin/*
