
CFLAGS= -Wall
CFLAGS+=-I./include


LIBS=-lpthread 
LIBS+=-lrt 
LIBS+=-ljansson 
LIBS+=-lcsfml-window -lcsfml-graphics -lcsfml-system 
LIBS+=-lm

ifeq ($(D), 1)
	CFLAGS +=-DDEBUG
	CFLAGS +=-g
endif

all: bin/main bin/inventoryManager

###########################
#		   ARNAUD		  #
###########################
bin/main.o: src/main.c 
	gcc $(CFLAGS) -c $^ -o $@ 

bin/robot.o:  src/robot.c 
	gcc $(CFLAGS) -c $^ -o $@ 

bin/sections.o:  src/sections.c 
	gcc $(CFLAGS) -c $^ -o $@ 

bin/environnement.o:  src/environnement.c 
	gcc $(CFLAGS) -c $^ -o $@ 


bin/main : bin/sections.o bin/main.o bin/robot.o bin/environnement.o bin/server.o bin/order.o
	gcc $(CFLAGS) $^ -o $@ $(LIBS)



###########################
#		   MAIN			  #
###########################

bin/mainRomain: bin/mainRomain.o bin/server.o bin/order.o
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/mainRomain.o: src/mainRomain.c
	gcc $(CFLAGS) -c $^ -o $@ $(LIBS)



###########################
#		   SERVER		  #
###########################

bin/server: src/server.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/server.o: src/server.c
	gcc $(CFLAGS) -c $^ -o $@ 



###########################
#	 INVENTORY MANAGER	  #
###########################

bin/inventoryManager: src/inventoryManager.cpp
	g++  $(CFLAGS) $^ -o $@ -lncurses -lpthread -std=c++17 $(LIBS)


###########################
#      	   ORDER		  #
###########################

bin/order.o: src/order.c
	gcc $(CFLAGS) -c $^ -o $@ -lsfml-system

bin/orderTest: src/order.c
	gcc $(CFLAGS) -DDEBUG -DTEST_ORDER  $^ -o $@ $(LIBS) 

###########################
#		 CLEAN			  #
###########################	

clean: 
	@rm -f ./bin/*
