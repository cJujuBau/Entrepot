all : simulationTrajectoires

simulationTrajectoires : simulationTrajectoires.c
	gcc $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics -lm

clean:
	rm -f simulationTrajectoires