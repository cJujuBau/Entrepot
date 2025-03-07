/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - reseau.h                         */
/*                        Auteur : CHEVALIER Romain                         */
/*                            Date : 26-10-2024                             */
/* ------------------------------------------------------------------------ */


#ifndef __RESEAUCLIENT_H
#define __RESEAUCLIENT_H 1

/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */

#include <utils.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */


#define PORT 3000

/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */





/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */

void initReseauClient(int *sd, const char *ipServeur);
int sendToServer(int sd, const int id, const char *msg, const int size);
int closeReseauClient(int sd);

void *threadReceptionReseau(void *arg);
void sendObstacleDetected(const char *buffer, int size);


/* ------------------------------------------------------------------------ */
/*                 V A R I A B L E S    G L O B A L E S                     */
/* ------------------------------------------------------------------------ */

extern int sd;
extern int receptionReseauEnCours;


#endif




