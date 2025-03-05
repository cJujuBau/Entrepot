CFLAGS= -Wall
CFLAGS+=-I./include
CSTFLAGS =-DDEBUG 


LIBS=-lpthread

ifeq ($(PC), 1)
	CFLAGS +=-DPC
	CFLAGS +=-DDEBUG
endif

all: bin/main

###########################
#		   MAIN			  #
###########################

bin/main: bin/main.o bin/serial.o bin/marvelmind.o bin/marvelmindClient.o bin/reseauClient.o
	gcc -g $(CFLAGS) $^ -o $@ $(LIBS)

bin/serial.o: src/serial.c
	gcc -g $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/marvelmind.o: src/marvelmind.c
	gcc -g $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/marvelmindClient.o: src/marvelmindClient.c
	gcc -g $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/reseauClient.o: src/reseauClient.c
	gcc -g $(CFLAGS) -c $^ -o $@ $(LIBS)

bin/main.o: src/main.c
	gcc -g $(CFLAGS) -c $^ -o $@ $(LIBS)

###########################
#		   SERIAL		  #
###########################	


bin/serialDEBUG: src/serial.c
	gcc $(CFLAGS) $(CSTFLAGS) $^ -o $@ $(LIBS)

bin/serial: src/serial.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

###########################
#		 MARVELMIND		  #
###########################

bin/marvelmindClient: src/marvelmind.c  src/marvelmindClient.c 
	gcc $(CFLAGS) $^ -o $@ $(LIBS)


###########################
#		 RESEAU			  #
###########################

bin/reseauClient: src/reseauClient.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)


###########################
#		 CLEAN			  #
###########################	

clean: 
	@rm -f ./bin/*







