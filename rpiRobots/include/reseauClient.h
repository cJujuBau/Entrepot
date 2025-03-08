/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - reseau.h                       */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */


#ifndef __RESEAUCLIENT_H
#define __RESEAUCLIENT_H 1

/* ------------------------------------------------------------------------ */
/*                        S T A N D A R D   H E A D E R S                   */
/* ------------------------------------------------------------------------ */

#include <utils.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* ------------------------------------------------------------------------ */
/*                   S Y M B O L I C   C O N S T A N T S                    */
/* ------------------------------------------------------------------------ */


#define PORT 3000

/* ------------------------------------------------------------------------ */
/*                        T Y P E   D E F I N I T I O N S                   */
/* ------------------------------------------------------------------------ */





/* ------------------------------------------------------------------------ */
/*                    F U N C T I O N   P R O T O T Y P E S                 */
/* ------------------------------------------------------------------------ */

void initReseauClient(int *sd, const char *ipServer);
int sendToServer(int sd, const int id, const char *msg, const int size);
int closeNetworkClient(int sd);

void *threadReceptionReseau(void *arg);
void sendObstacleDetected(const char *buffer, int size);


/* ------------------------------------------------------------------------ */
/*                        G L O B A L   V A R I A B L E S                   */
/* ------------------------------------------------------------------------ */

extern int sd;
extern int networkReceptionON;


#endif
