
CFLAGS= -Wall
CFLAGS+=-I./include


LIBS=-lpthread
LIBS+=-lrt

ifeq ($(D), 1)
	CFLAGS +=-DDEBUG
	CFLAGS +=-g
endif

all: bin/main bin/inventoryManager


###########################
#		   MAIN			  #
###########################

bin/main: bin/main.o bin/server.o bin/order.o
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/main.o: src/main.c
	gcc $(CFLAGS) -c $^ -o $@ $(LIBS)



###########################
#		   SERVER		  #
###########################

bin/server: src/server.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/server.o: src/server.c
	gcc $(CFLAGS) -c $^ -o $@ $(LIBS)



###########################
#	 INVENTORY MANAGER	  #
###########################

bin/inventoryManager: src/inventoryManager.cpp
	g++  $(CFLAGS) $^ -o $@ -lncurses -lpthread -std=c++17 $(LIBS)


###########################
#      	   ORDER		  #
###########################

bin/order.o: src/order.c
	gcc $(CFLAGS) -c $^ -o $@ $(LIBS)

###########################
#		 CLEAN			  #
###########################	

clean: 
	@rm -f ./bin/*
