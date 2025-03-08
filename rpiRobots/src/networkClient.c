#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <serial.h>
#include <main.h>
#include <networkClient.h>

int sd =-1;
int networkReceptionON = 1;
extern int id;

void initnetworkClient(int *sd, const char *ipServer){
    
    struct sockaddr_in addrServ;

    // Step 1 - Create the socket
    CHECK(*sd = socket(AF_INET, SOCK_STREAM, 0) ,"initnetworkClient: socket(AF_INET, SOCK_STREAM, 0)");

    // Step 2 - Addressing the recipient

    addrServ.sin_family=AF_INET;
    addrServ.sin_port=htons(PORT);
    addrServ.sin_addr.s_addr=inet_addr(ipServer);
    

    // Step 3 - Request to open a connection
    CHECK(connect(*sd, (const struct sockaddr *)&addrServ, sizeof(struct sockaddr_in)), "initnetworkClient: connect(sd)");
    sendToServer(*sd, id, "i;", 2);

}

int sendToServer(int sd, const int id, const char *msg, const int size){
    int nbChar;
    int NewSize = size+2;
    char buffer[NewSize];

    sprintf(buffer, "%d-%s", id, msg);
    nbChar = send(sd, buffer, NewSize+1, 0);

    DEBUG_PRINT("sendToServer: nbChar=%d, buffer=%s\n", nbChar, buffer);

    return nbChar-NewSize;
}

int closeNetworkClient(int sd){
    return close(sd);
}

void sendObstacleDetected(const char *buffer, int size){
    int sent =  sendToServer(sd, id, buffer, size);
    if (sent < 0)
    {
        perror("sendObstacleDetected: sendToServer failed");
    }
}

void *threadReceptionReseau(void *arg){
    char buffer[100];
    int nbChar;
    while (networkReceptionON)
    {
        nbChar = recv(sd, buffer, 100, 0);
        if (nbChar > 0)
        {
            buffer[nbChar] = '\0';
            DEBUG_PRINT("threadReceptionReseau: buffer=%s\n", buffer);
            
            if (buffer[0] == 'r' && raspiOn) writeSerial(portArduino, buffer, nbChar);
        }
        else if (nbChar == 0){ // Server closed the connection
            DEBUG_PRINT("threadReceptionReseau: Server closed the connection\n");
            networkReceptionON = 0; 
            break;
        }
        else // Error case
        {
            perror("threadReceptionReseau: recv failed");
            networkReceptionON = 0; 
            break;
        }
    }

    DEBUG_PRINT("threadReceptionReseau: End of thread\n");

    return NULL;
}



/* 

// Example of usage:

int main(int argc, char const *argv[])
{
    int sd=-1;
    
    initnetworkClient(&sd, "127.0.0.1");
    CHECK(sendToServer(sd, 1, "Hello", 7), "main: sendToServer failed");



    closeNetworkClient(sd);
    return 0;
}

*/ 