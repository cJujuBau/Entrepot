/* ------------------------------------------------------------------------ */
/*                 Entrepot - Robots - marvelmindClient.h                   */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */

#ifndef __MARVELMIND_CLIENT_H
#define __MARVELMIND_CLIENT_H 1

/* ------------------------------------------------------------------------ */
/*                        S T A N D A R D   H E A D E R S                   */
/* ------------------------------------------------------------------------ */

#include <utils.h>
#include <marvelmind.h>

#include <semaphore.h>
#include <time.h>

/* ------------------------------------------------------------------------ */
/*                      S Y M B O L I C   C O N S T A N T S                 */
/* ------------------------------------------------------------------------ */

#define MAX_COORD 50000
#define MIN_COORD -50000

/* ------------------------------------------------------------------------ */
/*                        T Y P E   D E F I N I T I O N S                   */
/* ------------------------------------------------------------------------ */

typedef struct RobotMarvelmind{
    struct MarvelmindHedge * hedge;
    int address;
    
} *RobotMarvelmind;

typedef struct Position{
    int32_t x, y;
    int32_t angle;
} *Position;



/* ------------------------------------------------------------------------ */
/*                       G L O B A L   V A R I A B L E S                    */
/* ------------------------------------------------------------------------ */
static sem_t *semMM;
extern struct timespec tsMM;


extern int addressMM;

extern RobotMarvelmind robotMarvelmind;
extern int getPostionON;

/* ------------------------------------------------------------------------ */
/*                      F U N C T I O N   P R O T O T Y P E S               */
/* ------------------------------------------------------------------------ */


void initRobotMarvelmind(RobotMarvelmind robotMarvelmind, const char* ttyFileName, const int address);
void destroyRobotMarvelmind(RobotMarvelmind robotMarvelmind);
void getPositionMarvelmind(RobotMarvelmind robotMarvelmind, Position position);
void printPositionMarvelmindRobot(RobotMarvelmind robotMarvelmind);

void *threadGetAndSendPositionMarvelmind(void *arg);

int encodePosition(char buffer[20], const Position position);

#endif