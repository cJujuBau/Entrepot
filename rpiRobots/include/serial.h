/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - serial.h                       */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */


#ifndef __SERIAL_H
#define __SERIAL_H 1

/* ------------------------------------------------------------------------ */
/*                        S T A N D A R D   H E A D E R S                   */
/* ------------------------------------------------------------------------ */

#include <utils.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include <pthread.h>

/* ------------------------------------------------------------------------ */
/*                      S Y M B O L I C   C O N S T A N T S                 */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*                        T Y P E   D E F I N I T I O N S                   */
/* ------------------------------------------------------------------------ */





/* ------------------------------------------------------------------------ */
/*                      F U N C T I O N   P R O T O T Y P E S               */
/* ------------------------------------------------------------------------ */
int openSerialPort(const char* portStr);
void setSerialPort(int port);
void writeSerial(int port, const char* data, const int size);
int readSerial(int port, char* buffer, int size);
void closeSerialPort(int port);

void *threadReceptionSerie(void *arg);

/* ------------------------------------------------------------------------ */
/*                       G L O B A L   V A R I A B L E S                    */
/* ------------------------------------------------------------------------ */


extern int portArduino;
extern pthread_mutex_t mutexSerialPort;
extern int serialReceptionON;

#endif