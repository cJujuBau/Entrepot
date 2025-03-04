


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <serial.h>

# define CRTSCTS 020000000000 /* Flow control.  */


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

void writeSerial(int port, const char* data){
    CHECK(write(port, data, strlen(data)), "msg: Write to serial failed!");
}

void readSerial(int port, char* buffer, int size){
    char single_char;
    int index = 0;
    while (1)
    {
        int reception = read(port, &single_char, 1); // Lire 1 caractère
        if (reception > 0)
        {
            buffer[index++] = single_char;
            DEBUG_PRINT("Caractere recu : %c\n", single_char); 

            if (single_char == '\n' || index >= size - 1) break;

        }
        else if (reception == -1)
        {
            perror("Erreur lecture port serie");
            break;
        }
    }

    // Terminer la chaîne de caractères
    buffer[index] = '\0';

    // Afficher le message reçu
    if (index > 0)
    {
        printf("Message reçu : %s\n", buffer);
    }
    else
    {
        printf("Aucune donnee recue.\n");
    }

    DEBUG_PRINT("Reception finie.\n");
}

void closeSerialPort(int port){
    CHECK(close(port), "msg: Unable to close serial port");
}

