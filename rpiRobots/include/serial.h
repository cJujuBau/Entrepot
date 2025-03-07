/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - serial.h                         */
/*                        Auteur : CHEVALIER Romain                         */
/*                            Date : 26-10-2024                             */
/* ------------------------------------------------------------------------ */


#ifndef __SERIAL_H
#define __SERIAL_H 1

/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */

#include <utils.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include <pthread.h>

/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */





/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */
int openSerialPort(const char* portStr);
void setSerialPort(int port);
void writeSerial(int port, const char* data, const int size);
int readSerial(int port, char* buffer, int size);
void closeSerialPort(int port);

void *threadReceptionSerie(void *arg);
/* ------------------------------------------------------------------------ */
/*                 V A R I A B L E S    G L O B A L E S                     */
/* ------------------------------------------------------------------------ */

// int portArduino =-1;
// pthread_mutex_t mutexSerialPort = PTHREAD_MUTEX_INITIALIZER;
// int receptionSerieEnCours = 1;

extern int portArduino;
extern pthread_mutex_t mutexSerialPort;
extern int receptionSerieEnCours;

#endif