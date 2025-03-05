#include <marvelmindClient.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>




bool terminateProgram=false;


void CtrlHandler(int signum)
{
    terminateProgram=true;
}



static sem_t *sem;
struct timespec ts;
void semCallback()
{
	sem_post(sem);
}

void initRobotMarvelmind(RobotMarvelmind robotMarvelmind, const char* ttyFileName, const int address){

    CHECK_NULL(sem = sem_open(DATA_INPUT_SEMAPHORE, O_CREAT, 0777, 0), "initRobotMarvelmind: sem_open(DATA_INPUT_SEMAPHORE)");

    robotMarvelmind->address = address;
    robotMarvelmind->hedge = createMarvelmindHedge();
    CHECK_NULL(robotMarvelmind->hedge, "Error: Unable to create MarvelmindHedge");

    robotMarvelmind->hedge->ttyFileName = ttyFileName;
    robotMarvelmind->hedge->verbose = true; // show errors and warnings
    robotMarvelmind->hedge->anyInputPacketCallback = semCallback;
    startMarvelmindHedge(robotMarvelmind->hedge);
}

void destroyRobotMarvelmind(RobotMarvelmind robotMarvelmind){
    stopMarvelmindHedge(robotMarvelmind->hedge);
    destroyMarvelmindHedge(robotMarvelmind->hedge);
    free(robotMarvelmind);

    sem_close(sem);
    sem_unlink(DATA_INPUT_SEMAPHORE);
}



void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position){
    struct MarvelmindHedge * hedge = robotMarvelmind->hedge;
    struct PositionValue positionMM;

    if (hedge->haveNewValues_){

        getPositionFromMarvelmindHedgeByAddress(hedge, &positionMM, robotMarvelmind->address);
        
        if (positionMM.ready==true ){ // Voir s'il faut mettre un max et min sur le svaleurs
            position->x= positionMM.x;
            position->y= positionMM.y;
            
            hedge->haveNewValues_=false;
        }
    }
}

// A supprimer comme fonction
void printPositionMarvelmindRobot(RobotMarvelmind robotMarvelmind){
    Position position = malloc(sizeof(struct Position));

    while ((!terminateProgram) && (!robotMarvelmind->hedge->terminationRequired))
    {
        
        if (clock_gettime(0, &ts) == -1)
		{
			printf("clock_gettime error");
			exit(EXIT_FAILURE);
		}
		ts.tv_sec += 2; // Voir si on peut pas virer ca 
		sem_timedwait(sem,&ts);


        getPositionMarvelmind(robotMarvelmind, position);
        DEBUG_PRINT("Position x: %d, y: %d\n", position->x, position->y);

    }

}

/*
// Bien verifier que le marvelmind soit defini en 115200 en baudrate, sinon voir sur le dashboard dans interface

int main (int argc, char *argv[])
{
    // Set Ctrl-C handler

    signal (SIGINT, CtrlHandler);
    signal (SIGQUIT, CtrlHandler);


    // Create RobotMarvelmind
    robotMarvelmind = malloc(sizeof(struct RobotMarvelmind));
    initRobotMarvelmind(robotMarvelmind, PORT_MARVELMIND, ADDRESS_MM);
    printPositionMarvelmindRobot(robotMarvelmind);



    destroyRobotMarvelmind(robotMarvelmind);
    exit(EXIT_SUCCESS);
    return 0;
}
*/