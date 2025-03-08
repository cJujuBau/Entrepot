/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - main.h                         */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */


#ifndef __MAIN_H
#define __MAIN_H 1

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

#define PORT_ARDUINO "/dev/ttyS0"
#define PORT_MARVELMIND "/dev/ttyACM0"
#define PATH_PARAM "./param.json"


/* ------------------------------------------------------------------------ */
/*                        T Y P E   D E F I N I T I O N S                   */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*                        G L O B A L   V A R I A B L E S                   */
/* ------------------------------------------------------------------------ */

extern int id;
extern int raspiOn; // If compiled on the Raspberry Pi connected to the Arduino
extern int marvelmindOn; // If compiled on the Raspberry Pi connected to the Marvelmind


/* ------------------------------------------------------------------------ */
/*                    F U N C T I O N   P R O T O T Y P E S                 */
/* ------------------------------------------------------------------------ */

void initParam();
void bye();
void *threadReceptionReseau(void *arg);

#endif