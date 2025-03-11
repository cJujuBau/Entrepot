# filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/makefile
CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lm

all : bin/main

bin/main.o: src/main.c 
	gcc $(CFLAGS) -c $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

bin/robot.o:  src/robot.c 
	gcc $(CFLAGS) -c $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

bin/sections.o:  src/sections.c 
	gcc $(CFLAGS) -c $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

bin/environnement.o:  src/environnement.c 
	gcc $(CFLAGS) -c $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

bin/utils.o:  src/utils.c 
	gcc $(CFLAGS) -c $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

bin/main : bin/sections.o bin/main.o bin/robot.o bin/environnement.o bin/utils.o
	gcc $(CFLAGS) $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

sim: main.c robot.c environnement.c sections.c utils.c
	gcc $(CFLAGS) $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm


clean:
	rm -rf ./bin/*

.PHONY: all clean

# all : simulationTrajectoires

simulationTrajectoires : simulationTrajectoires.c 
	gcc $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

# clean:
# 	rm -f simulationTrajectoires