#include <marvelmindClient.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <utils.h>
#include <networkClient.h>
#include <serial.h>
#include <main.h>

struct timespec tsMM;
int addressMM = 0;

RobotMarvelmind robotMarvelmind = NULL;
int getPositionON = 1;

bool terminateProgram=false;

// Marvelmind parameters : xMM = 3.9*y - 1700
//                         yMM = -1.4*x - 200
//                         x = - 70/100 * (yMM + 200)
//                         y = 26/100 *(xMM + 1700)

#define gainX  1650// 0.70 But we multiply by 100 to avoid float
#define offsetX 2500

#define gainY 1000  // 0.26 But we multiply by 10 to avoid float
#define offsetY 3800


void transformPosition(Position position){
    int x = position->x;
    position->x = - gainX * (position->y - offsetX) / offsetX;
    position->y = - gainY * (x - offsetY) / offsetY;
}



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
        // Create MarvelmindHedge same as in example on this link : https://github.com/MarvelmindRobotics/marvelmind.c/tree/master/src

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
    if (marvelmindOn){
        stopMarvelmindHedge(robotMarvelmind->hedge);
        destroyMarvelmindHedge(robotMarvelmind->hedge);
    }

    free(robotMarvelmind);

    sem_close(semMM);
    sem_unlink(DATA_INPUT_SEMAPHORE);
}


// TO TEST
void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position){
    struct MarvelmindHedge * hedge = robotMarvelmind->hedge;
    struct PositionValue positionMM;

    if (marvelmindOn){
        if (!robotMarvelmind->hedge->terminationRequired){
            if (clock_gettime(0, &tsMM) == -1){       
                printf("clock_gettime error");
                exit(EXIT_FAILURE);
            }

            tsMM.tv_sec += 2; // See if we can remove this
            sem_timedwait(semMM,&tsMM);


            if (hedge->haveNewValues_){
                // Same as printPositionFromMarvelmindHedge in marvelmind.c file
                getPositionFromMarvelmindHedgeByAddress(hedge, &positionMM, robotMarvelmind->address);
                
                if (positionMM.ready==true && positionMM.x < MAX_COORD && positionMM.x > MIN_COORD){

                    
                    // position->x= 2000 - positionMM.x;
                    // position->y= 1300 - positionMM.y;

                    //DEBUG_PRINT("getPositionMarvelmind: x=%d, y=%d\n", position->x, position->y);
                    
                    if (position->x == 0 || position->y == 0){
                        position->x= 2000 - positionMM.x;
                        position->y= 1300 - positionMM.y;
                    } else {
                        position->x = 0.8 * (2000 - positionMM.x) + (1-0.8) * position->x;
                        position->y = 0.8 * (1300 - positionMM.y) + (1-0.8) * position->y;
                        // 0.8 = coefAverageMovingFilter
                    }
                   
                    
                    hedge->haveNewValues_=false;
                }
            }
        } 
        else getPositionON = 0;


    } else {
        sleep(1);
        position->x = 1000 + rand() % 1000;
        position->y = 1000 + rand() % 1000;
    }    
}

// Encoding position to send to the server and serial port
int encodePosition(char buffer[20], const Position position){
    sprintf(buffer, "p:%d,%d;", position->x, position->y);
    return strlen(buffer);
}


#ifndef TEST_MM

// Thread function to get and send position to the server and serial port
void *threadGetAndSendPositionMarvelmind(void *arg){
    Position position = malloc(sizeof(struct Position));
    Position newPosition = malloc(sizeof(struct Position));
    Position positionSever = malloc(sizeof(struct Position));
    int size = -1;
    char buffer[20];
    struct timespec ts, newTs;

    CHECK(clock_gettime(0, &ts), "threadGetAndSendPositionMarvelmind: clock_gettime(ts) failed");

    while (getPositionON)
    {           
        // sleep(1);
        //usleep(500000); // 500ms -> Force 2Hz frequency // MM 14 has new values every 1ms but it doesn't change and server crashes
        getPositionMarvelmind(robotMarvelmind, newPosition);
        CHECK(clock_gettime(0, &newTs), "threadGetAndSendPositionMarvelmind: clock_gettime(newTs) failed");
        // Force to only change position if it is different
        if (newPosition->x == position->x && newPosition->y == position->y && (newTs.tv_sec - ts.tv_sec) < 1){
            continue;
        }

        // Update position
        position->x = newPosition->x;
        position->y = newPosition->y;

        // Update time
        ts.tv_sec = newTs.tv_sec;
        ts.tv_nsec = newTs.tv_nsec;

        DEBUG_PRINT("threadGetAndSendPositionMarvelmind: x=%d, y=%d\n", position->x, position->y);

        size = encodePosition(buffer, position);
        DEBUG_PRINT("threadGetAndSendPositionMarvelmind: sentBuffer=%s\n", buffer);
        
        // Serial write
        if (raspiOn){
            pthread_mutex_lock(&mutexSerialPort);   
            writeSerial(portArduino, buffer, size); 
            pthread_mutex_unlock(&mutexSerialPort); 
        }
        
        // // Network write
        
        positionSever->x = position->x;
        positionSever->y = position->y;
        transformPosition(positionSever); // Changement de repere
        size = encodePosition(buffer, positionSever);
        
        
        if (sendToServer(sd, id, buffer, size) < 0){
            perror("threadGetAndSendPositionMarvelmind: sendToServer failed");
            getPositionON = 0;
        }
        
    }

    DEBUG_PRINT("threadGetAndSendPositionMarvelmind: End of thread\n");
    free(position);
    free(positionSever);
    
    return NULL;
}




#endif





















#ifdef TEST_MM

// TO TEST THE FILE ONLY 

// To be removed as function
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
        tsMM.tv_sec += 2; // See if we can remove this
        sem_timedwait(semMM,&tsMM);

        getPositionMarvelmind(robotMarvelmind, position);
        DEBUG_PRINT("Position x: %d, y: %d\n", position->x, position->y);

    }

}



#define PORT_MARVELMIND "/dev/ttyACM0"
#define ADDRESS_MM      12
RobotMarvelmind robotMarvelmind;
// Make sure the marvelmind is set to 115200 baudrate, otherwise check on the dashboard in interface

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

