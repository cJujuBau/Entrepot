CFLAGS= -Wall
CFLAGS+=-I./include

ifeq ($(D), 1)
	CFLAGS +=-DDEBUG
	CFLAGS +=-g
endif

LIBS= `pkg-config --cflags --libs opencv4`



all: bin/detection 

###########################
#		DETECTION		  #
###########################																

bin/detection: src/detection.cpp src/ptz.c 
	g++ $(CFLAGS) $^ -o $@  $(LIBS)



###########################
#		   MAIN			  #
###########################	


bin/main: src/main.c 
	gcc $(CFLAGS) $^ -o $@ $(LIBS)



###########################
#		 CLEAN			  #
###########################	

clean: resetLog cleanServeur
	@rm -f ./bin/*








