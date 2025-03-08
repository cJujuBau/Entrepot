/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - main.h                         */
/*                        Auteur : CHEVALIER Romain                         */
/*                            Date : 26-10-2024                             */
/* ------------------------------------------------------------------------ */


#ifndef __MAIN_H
#define __MAIN_H 1

/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */

#include <utils.h>

// Signaux
#include <signal.h>
#include <bits/sigaction.h> // Include pour evider les erreurs sur vscode 
#include <bits/types/sigset_t.h>

/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */

#define PORT_ARDUINO "/dev/ttyS0"
#define PORT_MARVELMIND "/dev/ttyACM0"
#define PATH_PARAM "./param.json"


/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*                 V A R I A B L E S    G L O B A L E S                     */
/* ------------------------------------------------------------------------ */

extern int id;
extern int raspiOn; // Si on complie sur le raspberry connecté à l'Arduino
extern int marvelmindOn; // Si on compile sur le raspberry connecté au marvelmind


/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */


void *threadReceptionReseau(void *arg);

#endif