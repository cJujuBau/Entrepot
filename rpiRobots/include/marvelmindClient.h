#include <utils.h>
#include <marvelmind.h>

typedef struct RobotMarvelmind{
    struct MarvelmindHedge * hedge;
    
} *RobotMarvelmind;

typedef struct Position{
    int32_t x,y, z;
} *Position;

void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position);
void initRobotMarvelmind(RobotMarvelmind robotMarvelmind, const char* ttyFileName);
void destroyRobotMarvelmind(RobotMarvelmind robotMarvelmind);
void printPositionMarvelmindRobot(RobotMarvelmind robotMarvelmind);

