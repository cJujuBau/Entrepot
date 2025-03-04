all : simulationTrajectoires

simulationTrajectoires : simulationTrajectoires.c
	gcc $^ -o $@ -lcsfml-window -lcsfml-system -lcsfml-graphics

clean:
	rm -f simulationTrajectoires