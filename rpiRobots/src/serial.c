


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <serial.h>
#include <reseauClient.h>

# define CRTSCTS 020000000000 /* Flow control.  */


#define FIRST_CHAR_HUMAN_READABLE 32

int portArduino = -1;                      // Définition unique
pthread_mutex_t mutexSerialPort = PTHREAD_MUTEX_INITIALIZER;
int receptionSerieEnCours = 1;


int openSerialPort(const char* portStr) {
    int port;
    CHECK(port = open(portStr, O_RDWR | O_NOCTTY | O_NDELAY), "msg: Unable to open serial");
    fcntl(port, F_SETFL, 0);
    return port;
}

void setSerialPort(int port){
    struct termios options;
    tcgetattr(port, &options);

    // Set the baud rates to 115200...
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    // Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Disable hardware flow control
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Configurer VMIN et VTIME pour un délai après le premier caractère
    options.c_cc[VMIN] = 1;  // Ne pas attendre un nombre minimum de caractères
    options.c_cc[VTIME] = 10; // Attendre jusqu'à 1 secondes après le premier caractère

    
    CHECK(tcsetattr(port, TCSANOW, &options), "msg: Unable to set serial port");
}

void writeSerial(int port, const char* data, const int size){
    int new_size = size + 2;
    char buffer[new_size];
    sprintf(buffer, "%s\r\n", data);

    CHECK(write(port, buffer, new_size), "writeSerial: Write to serial failed!");
}

int readSerial(int port, char* buffer, int size){
    char single_char;
    int index = 0;
    int reception = 0;
    char buffTemp[size];

    while (1)
    {
        reception = read(port, &single_char, 1); // Lire 1 caractère
        if (reception > 0)
        {
            // DEBUG_PRINT("readSerial : Caractere recu : %c et %d\n", single_char, single_char);
            if (single_char >= FIRST_CHAR_HUMAN_READABLE ) buffTemp[index++] = single_char;
            if (single_char == '\n' || index > size) break;

        }
        else if (reception == -1)
        {
            perror("Erreur lecture port serie");
            return -1;
        }
    }

    // Terminer la chaîne de caractères
    // DEBUG_PRINT("readSerial: index=%d, buffer=%s\n", index, buffer);

    if (index > 0) {
        buffTemp[index] = '\0';
        strcpy(buffer, buffTemp);
    }

    // Afficher le message reçu
    // if (index > 0) DEBUG_PRINT("Message reçu : %s\n", buffer);
    // else printf("Aucune donnee recue.\n");

    // DEBUG_PRINT("Reception finie.\n");
    return index;
}
void *threadReceptionSerie(void *arg){
    char buffer[100];
    int nbChar = 0;
    while (receptionSerieEnCours)
    {
        nbChar = readSerial(portArduino, buffer, 100); // Voir si ca ne bloque pas le port serie et mettre la mutex
        // DEBUG_PRINT("threadReceptionSerie: buffer=%s, nbCharRecu=%d\n", buffer, nbChar);
        if (nbChar > 0)
        {
            DEBUG_PRINT("threadReceptionSerie: buffer=%s\n", buffer);
            if (buffer[0] == 'o') sendObstacleDetected(buffer, nbChar);
        }
    }

    return NULL;
}


void closeSerialPort(int port){
    CHECK(close(port), "msg: Unable to close serial port");
}




#ifdef TEST_SERIAL


// Exemple d'utilisation :
#define PORT_ARDUINO "/dev/ttyS0"

int main(int argc, char const *argv[])
{
    int port = openSerialPort(PORT_ARDUINO);
    setSerialPort(port);
    writeSerial(port, "r:10,100,25;\r\n", 16);
    // char buffer[100];
    // readSerial(port, buffer, 100);
    closeSerialPort(port);  

    

    return 0;
}

#endif
