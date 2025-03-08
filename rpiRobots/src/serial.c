#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <serial.h>
#include <reseauClient.h>

# define CRTSCTS 020000000000 /* Flow control.  for VsCode*/

#define FIRST_CHAR_HUMAN_READABLE 32

int portArduino = -1;                     
pthread_mutex_t mutexSerialPort = PTHREAD_MUTEX_INITIALIZER;
int serialReceptionON = 1;

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

    // Configure VMIN and VTIME for a delay after the first character
    options.c_cc[VMIN] = 1;  // Wait for at least 1 character
    options.c_cc[VTIME] = 10; // Wait up to 1 second after the first character

    CHECK(tcsetattr(port, TCSANOW, &options), "msg: Unable to set serial port");
}

void writeSerial(int port, const char* data, const int size){
    int new_size = size + 2;
    char buffer[new_size];
    sprintf(buffer, "%s\r\n", data);

    CHECK(write(port, buffer, new_size), "writeSerial: Write to serial failed!");
}

int readSerial(int port, char* buffer, int size){
    char singleChar;
    int index = 0;
    int reception = 0;
    char buffTemp[size];

    while (1)
    {
        reception = read(port, &singleChar, 1); // Read 1 character
        if (reception > 0)
        {
            // DEBUG_PRINT("readSerial : Character received : %c and %d\n", singleChar, singleChar);
            if (singleChar >= FIRST_CHAR_HUMAN_READABLE ) buffTemp[index++] = singleChar;
            if (singleChar == '\n' || index > size) break;
        }
        else if (reception == -1)
        {
            perror("Error reading serial port");
            return -1;
        }
    }

    // Terminate the string
    // DEBUG_PRINT("readSerial: index=%d, buffer=%s\n", index, buffer);

    if (index > 0) {
        buffTemp[index] = '\0';
        strcpy(buffer, buffTemp);
    }

    return index;
}

void *threadReceptionSerie(void *arg){
    char buffer[100];
    int nbChar = 0;
    while (serialReceptionON)
    {
        nbChar = readSerial(portArduino, buffer, 100);
        // DEBUG_PRINT("threadReceptionSerie: buffer=%s, nbCharRecu=%d\n", buffer, nbChar);

        if (nbChar > 0){
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

// Example usage:
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
