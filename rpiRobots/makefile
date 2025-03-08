CFLAGS= -Wall
CFLAGS+=-I./include
CSTFLAGS =-DDEBUG 


LIBS=-lpthread

ifeq ($(D), 1)
	CFLAGS +=-DDEBUG
	CFLAGS +=-g
endif


all: bin/main

###########################
#		   MAIN			  #
###########################

bin/main: bin/main.o bin/serial.o bin/marvelmind.o bin/marvelmindClient.o bin/networkClient.o
	gcc  $(CFLAGS) $^ -o $@ $(LIBS) -ljansson

bin/serial.o: src/serial.c
	gcc  $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/marvelmind.o: src/marvelmind.c
	gcc  $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/marvelmindClient.o: src/marvelmindClient.c
	gcc  $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/networkClient.o: src/networkClient.c
	gcc  $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/main.o: src/main.c
	gcc  $(CFLAGS) -c $^ -o $@ $(LIBS) -ljansson

###########################
#		   SERIAL		  #
###########################	


bin/serialTEST: src/serial.c
	gcc $(CFLAGS) $(CSTFLAGS) -DTEST_SERIAL $^ -o $@ $(LIBS)

bin/serial: src/serial.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

###########################
#		 MARVELMIND		  #
###########################

bin/marvelmindClient: src/marvelmind.c  src/marvelmindClient.c 
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/marvelmindClientTEST: src/marvelmind.c  src/marvelmindClient.c
	gcc $(CFLAGS) $(CSTFLAGS) -DTEST_MM $^ -o $@ $(LIBS)


###########################
#		 RESEAU			  #
###########################

bin/networkClient: src/networkClient.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)


###########################
#		 CLEAN			  #
###########################	

clean: 
	@rm -f ./bin/*







