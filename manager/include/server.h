/* ------------------------------------------------------------------------ */
/*                       Entrepot - Manager - server.h                      */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */


#ifndef __SERVER_H
#define __SERVER_H 1

/* ------------------------------------------------------------------------ */
/*                        S T A N D A R D   H E A D E R S                   */
/* ------------------------------------------------------------------------ */

#include <utils.h>

// Signals
#include <signal.h>
#include <bits/sigaction.h> // Include to avoid errors on vscode
#include <bits/types/sigset_t.h>

/* ------------------------------------------------------------------------ */
/*                    S Y M B O L I C   C O N S T A N T S                   */
/* ------------------------------------------------------------------------ */

#define MAXCAR 100
#define LOCALPORT 3000


/* ------------------------------------------------------------------------ */
/*                        T Y P E   D E F I N I T I O N S                   */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*                        G L O B A L   V A R I A B L E S                   */
/* ------------------------------------------------------------------------ */

extern int serverON;
extern int se;

/* ------------------------------------------------------------------------ */
/*                    F U N C T I O N   P R O T O T Y P E S                 */
/* ------------------------------------------------------------------------ */


void *threadConnexionClient(void *arg);
void *threadServer(void *arg);
void closeNetworkServer(int se);
void  initNetworkServer(int *se);
int processStringReceived(const char *buffer, const int size, int sd);
void obstacleDetected(int x, int y, int o);
int sendPosRef(int id);

void initRobots();
void freeRobots();
void printRobots();
void printRobot(int i);
void bye();


#endif




