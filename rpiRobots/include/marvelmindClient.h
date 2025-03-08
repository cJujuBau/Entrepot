#include <utils.h>
#include <marvelmind.h>

#include <semaphore.h>
#include <time.h>


#ifndef __MARVELMIND_CLIENT_H

#define __MARVELMIND_CLIENT_H 1

#define MAX_COORD 50000
#define MIN_COORD -50000

typedef struct RobotMarvelmind{
    struct MarvelmindHedge * hedge;
    int address;
    
} *RobotMarvelmind;

static sem_t *semMM;
extern struct timespec tsMM;


extern int addressMM;

extern RobotMarvelmind robotMarvelmind;
extern int getPostionOn;

void initRobotMarvelmind(RobotMarvelmind robotMarvelmind, const char* ttyFileName, const int address);
void destroyRobotMarvelmind(RobotMarvelmind robotMarvelmind);
void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position);
void printPositionMarvelmindRobot(RobotMarvelmind robotMarvelmind);

void *threadGetAndSendPositionMarvelmind(void *arg);

int encodePosition(char buffer[20], const Position position);

#endif