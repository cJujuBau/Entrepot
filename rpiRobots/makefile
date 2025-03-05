CFLAGS= -Wall
CFLAGS+=-I./include
CSTFLAGS =-DDEBUG 


LIBS=-lpthread

all: bin/serial

###########################
#		   SERIAL		  #
###########################	


bin/serialDEBUG: src/serial.c
	gcc $(CFLAGS) $(CSTFLAGS) $^ -o $@ $(LIBS)

bin/serial: src/serial.c
	gcc $(CFLAGS) $^ -o $@ $(LIBS)

bin/marvelmindClient: src/marvelmind.c  src/marvelmindClient.c 
	gcc $(CFLAGS) $^ -o $@ $(LIBS)


###########################
#		 CLEAN			  #
###########################	

clean: 
	@rm -f ./bin/*







