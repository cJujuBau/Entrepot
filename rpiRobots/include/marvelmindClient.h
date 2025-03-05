#include <utils.h>
#include <marvelmind.h>

#ifndef __MARVELMIND_CLIENT_H
#define __MARVELMIND_CLIENT_H

typedef struct RobotMarvelmind{
    struct MarvelmindHedge * hedge;
    int address;
    
} *RobotMarvelmind;



void initRobotMarvelmind(RobotMarvelmind robotMarvelmind, const char* ttyFileName, const int address);
void destroyRobotMarvelmind(RobotMarvelmind robotMarvelmind);
void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position);
void printPositionMarvelmindRobot(RobotMarvelmind robotMarvelmind);

#endif