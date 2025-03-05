#include <marvelmindClient.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>

#define ADDRESS_MM 11
#define TTY_FILENAME "/dev/ttyACM0"


bool terminateProgram=false;
RobotMarvelmind robotMarvelmind;

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

void initRobotMarvelmind(RobotMarvelmind robotMarvelmind, const char* ttyFileName){
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
}



void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position){
    
    // uint8_t i,j;
    
    struct MarvelmindHedge * hedge = robotMarvelmind->hedge;
    struct PositionValue positionMM;

    if (hedge->haveNewValues_)
    {

            getPositionFromMarvelmindHedgeByAddress(hedge, &positionMM, ADDRESS_MM);
            if (positionMM.ready==false)
            {
                // printf("Position not ready\n");
                return;
            }
            position->x= positionMM.x;
            position->y= positionMM.y;
            position->z= positionMM.z;
            
            hedge->haveNewValues_=false;
    }
    
   
}


void printPositionMarvelmindRobot(RobotMarvelmind robotMarvelmind){
    Position position = malloc(sizeof(struct Position));

    while ((!terminateProgram) && (!robotMarvelmind->hedge->terminationRequired))
    {
        
        if (clock_gettime(0, &ts) == -1)
		{
			printf("clock_gettime error");
			exit(EXIT_FAILURE);
		}
		ts.tv_sec += 4;
		sem_timedwait(sem,&ts);


        //printPositionFromMarvelmindHedge(robotMarvelmind->hedge, false);
        getPositionMarvelmind(robotMarvelmind, position);
        printf("Position x: %d, y: %d, z: %d\n", position->x, position->y, position->z);

    }

}

void bye()
{
    sem_close(sem);
    sem_unlink(DATA_INPUT_SEMAPHORE);
    destroyRobotMarvelmind(robotMarvelmind);
}

// Bien verifier que le marvelmind soit defini en 115200 en baudrate, sinon voir sur le dashboard dans interface

int main (int argc, char *argv[])
{

    atexit(bye);


    // Create RobotMarvelmind
    robotMarvelmind = malloc(sizeof(struct RobotMarvelmind));
    initRobotMarvelmind(robotMarvelmind, TTY_FILENAME);

    // Set Ctrl-C handler

    signal (SIGINT, CtrlHandler);
    signal (SIGQUIT, CtrlHandler);



	// Linux
	sem = sem_open(DATA_INPUT_SEMAPHORE, O_CREAT, 0777, 0);


    // Get position


    printPositionMarvelmindRobot(robotMarvelmind);

    exit(EXIT_SUCCESS);
    return 0;
}
