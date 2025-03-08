#include <marvelmindClient.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <utils.h>
#include <reseauClient.h>
#include <serial.h>
#include <main.h>

struct timespec tsMM;
int addressMM = 0;

RobotMarvelmind robotMarvelmind = NULL;
int getPostionOn = 1;

bool terminateProgram=false;


void CtrlHandler(int signum)
{
    terminateProgram=true;
}




void semCallback()
{
	sem_post(semMM);
}

void initRobotMarvelmind(RobotMarvelmind robotMarvelmind, const char* ttyFileName, const int address){

    CHECK_NULL(semMM = sem_open(DATA_INPUT_SEMAPHORE, O_CREAT, 0777, 0), "initRobotMarvelmind: sem_open(DATA_INPUT_SEMAPHORE)");

    robotMarvelmind->address = address;

    if (marvelmindOn){
        robotMarvelmind->hedge = createMarvelmindHedge();
        CHECK_NULL(robotMarvelmind->hedge, "Error: Unable to create MarvelmindHedge");

        robotMarvelmind->hedge->ttyFileName = ttyFileName;
        robotMarvelmind->hedge->verbose = true; // show errors and warnings
        robotMarvelmind->hedge->anyInputPacketCallback = semCallback;
        startMarvelmindHedge(robotMarvelmind->hedge);

        if (robotMarvelmind->hedge->terminationRequired)
        {
            puts("marvelmindClient.c : Error-Unable to start MarvelmindHedge");
            exit(EXIT_FAILURE);
        }
    } 
    else robotMarvelmind->hedge = NULL;
}

void destroyRobotMarvelmind(RobotMarvelmind robotMarvelmind){
    #ifndef PC
    stopMarvelmindHedge(robotMarvelmind->hedge);
    destroyMarvelmindHedge(robotMarvelmind->hedge);
    #endif

    free(robotMarvelmind);

    sem_close(semMM);
    sem_unlink(DATA_INPUT_SEMAPHORE);
}


// A TESTER
void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position){
    struct MarvelmindHedge * hedge = robotMarvelmind->hedge;
    struct PositionValue positionMM;

    if (marvelmindOn){
        if (!robotMarvelmind->hedge->terminationRequired){
            if (clock_gettime(0, &tsMM) == -1){       
                printf("clock_gettime error");
                exit(EXIT_FAILURE);
            }

            tsMM.tv_sec += 2; // Voir si on peut pas virer ca 
            sem_timedwait(semMM,&tsMM);


            if (hedge->haveNewValues_){
                getPositionFromMarvelmindHedgeByAddress(hedge, &positionMM, robotMarvelmind->address);
        
                if (positionMM.ready==true && positionMM.x < MAX_COORD && positionMM.x > MIN_COORD){
                    position->x= positionMM.x;
                    position->y= positionMM.y;
                    
                    hedge->haveNewValues_=false;
                }
            }
        } 
        else getPostionOn = 0;


    } else {
        sleep(1);
        position->x = 1000 + rand() % 1000;
        position->y = 1000 + rand() % 1000;
    }    
}

int encodePosition(char buffer[20], const Position position){
    sprintf(buffer, "p:%d,%d;", position->x, position->y);
    return strlen(buffer);
}


#ifndef TEST_MM

void *threadGetAndSendPositionMarvelmind(void *arg){
    Position position = malloc(sizeof(struct Position));
    int size = -1;
    char buffer[20];

    while (getPostionOn)
    {           
        getPositionMarvelmind(robotMarvelmind, position);
        DEBUG_PRINT("threadGetAndSendPositionMarvelmind: x=%d, y=%d\n", position->x, position->y);

        size = encodePosition(buffer, position);
        DEBUG_PRINT("threadGetAndSendPositionMarvelmind: sentBuffer=%s\n", buffer);
        
        // Ecriture serie
        if (raspiOn){
            pthread_mutex_lock(&mutexSerialPort);   
            writeSerial(portArduino, buffer, size); 
            pthread_mutex_unlock(&mutexSerialPort); 
        }
        
        // Ecriture reseau
        CHECK(sendToServer(sd, id, buffer, size), "threadGetAndSendPositionMarvelmind: sendToServer failed");
    }

    DEBUG_PRINT("threadGetAndSendPositionMarvelmind: End of thread\n");
    free(position);
    
    return NULL;
}




#endif





















#ifdef TEST_MM

// A supprimer comme fonction
void printPositionMarvelmindRobot(RobotMarvelmind robotMarvelmind){
    Position position = malloc(sizeof(struct Position));
    position->x = 0;
    position->y = 0;


    while ((!terminateProgram) && (!robotMarvelmind->hedge->terminationRequired))
    {
        
        if (clock_gettime(0, &tsMM) == -1)
            {       
                printf("clock_gettime error");
                exit(EXIT_FAILURE);
            }
        tsMM.tv_sec += 2; // Voir si on peut pas virer ca 
        sem_timedwait(semMM,&tsMM);

        getPositionMarvelmind(robotMarvelmind, position);
        DEBUG_PRINT("Position x: %d, y: %d\n", position->x, position->y);

    }

}



#define PORT_MARVELMIND "/dev/ttyACM0"
#define ADDRESS_MM      12
RobotMarvelmind robotMarvelmind;
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

#endif
