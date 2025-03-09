CFLAGS= -Wall
CFLAGS+=-I./include

ifeq ($(D), 1)
	CFLAGS +=-DDEBUG
	CFLAGS +=-g
endif

LIBS= `pkg-config --cflags --libs opencv4`
LIBS+= -lrealsense2



all: bin/objectDetection 

###########################
#		DETECTION		  #
###########################																

bin/objectDetection: src/objectDetection.cpp 
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








