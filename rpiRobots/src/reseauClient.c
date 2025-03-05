#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <reseauClient.h>

void initReseauClient(int *sd, const char *ipServeur){
    
    struct sockaddr_in addrServ;

    //Etape 1 - Creation de la socket
    CHECK(*sd = socket(AF_INET, SOCK_STREAM, 0) ,"initReseauClient: socket(AF_INET, SOCK_STREAM, 0)");

    //Etape2 - Adressage du destinataire

    addrServ.sin_family=AF_INET;
    addrServ.sin_port=htons(PORT);
    addrServ.sin_addr.s_addr=inet_addr(ipServeur);
    

    //Etape 3 - demande d'ouverture de connexion
    CHECK(connect(*sd, (const struct sockaddr *)&addrServ, sizeof(struct sockaddr_in)), "initReseauClient: connect(sd)");

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

int closeReseauClient(int sd){
    return close(sd);
}





/* 

// Exemple d'utilisation :

int main(int argc, char const *argv[])
{
    int sd=-1;
    
    initReseauClient(&sd, "127.0.0.1");
    CHECK(sendToServer(sd, 1, "Hello", 7), "main: sendToServer failed");



    closeReseauClient(sd);
    return 0;
}

*/ 