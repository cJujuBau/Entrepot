#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <serial.h>
#include <main.h>
#include <marvelmindClient.h>
#include <reseauClient.h>

#define PORT_ARDUINO    "/dev/ttyS0"
#define PORT_MARVELMIND "/dev/ttyACM0"
#define ADDRESS_MM      12
#define ID              1
#ifdef PC
#define IP_SERVEUR      "127.0.0.1"
#else
#define IP_SERVEUR      "172.20.10.7"// "127.0.0.1"
#endif

pthread_mutex_t mutexSerialPort = PTHREAD_MUTEX_INITIALIZER;



int sd =-1;
int portArduino =-1;

RobotMarvelmind robotMarvelmind;
int getPostionOn = 1;
int receptionReseauEnCours = 1;
int receptionSerieEnCours = 1;

// Faire une mutex pour ecrire sur le port serie



void *threadGetAndSendPositionMarvelmind(void *arg){
    Position positionTemp = malloc(sizeof(struct Position));

    while (getPostionOn)
    {
        #ifdef PC
            positionTemp->x = 1000 + rand() % 1000;
            positionTemp->y = 1000 + rand() % 1000;
        #else
        if (!robotMarvelmind->hedge->terminationRequired){

            if (clock_gettime(0, &tsMM) == -1)
            {       
                printf("clock_gettime error");
                exit(EXIT_FAILURE);
            }

            tsMM.tv_sec += 2; // Voir si on peut pas virer ca 
            sem_timedwait(semMM,&tsMM);
            getPositionMarvelmind(robotMarvelmind, positionTemp);
            
        } 
        else getPostionOn = 0;
        #endif 
        
        DEBUG_PRINT("threadGetAndSendPositionMarvelmind: x=%d, y=%d\n", positionTemp->x, positionTemp->y);
        
        
        char buffer[20];
        int size = -1;
        sprintf(buffer, "pos:%d:%d", positionTemp->x, positionTemp->y);
        size = strlen(buffer);
        DEBUG_PRINT("threadGetAndSendPositionMarvelmind: sentBuffer=%s\n", buffer);
        

        #ifndef PC
        pthread_mutex_lock(&mutexSerialPort);
        writeSerial(portArduino, buffer, size);
        pthread_mutex_unlock(&mutexSerialPort);

        #endif
        
        // Ecriture reseau
        CHECK(sendToServer(sd, ID, buffer, size), "threadGetAndSendPositionMarvelmind: sendToServer failed");
        
        
        #ifdef PC
        sleep(1); // Pas surd eca
        #endif
    }

    free(positionTemp);
    
    return NULL;
}

void *threadReceptionReseau(void *arg){
    char buffer[100];
    int nbChar;
    while (receptionReseauEnCours)
    {
        nbChar = recv(sd, buffer, 100, 0);
        if (nbChar > 0)
        {
            buffer[nbChar] = '\0';
            DEBUG_PRINT("threadReceptionReseau: buffer=%s\n", buffer);
        }
    }

    return NULL;
}

void *threadReceptionSerie(void *arg){
    char buffer[100];
    int nbChar;
    while (receptionSerieEnCours)
    {
        readSerial(portArduino, buffer, 100); // Voir si ca ne bloque pas le port serie et mettre la mutex
        DEBUG_PRINT("threadReceptionSerie: buffer=%s\n", buffer);
    }

    return NULL;
}



static void signalHandler(int numSig)
{ 
    switch(numSig) {
        case SIGINT : // traitement de SIGINT
            printf("\n\t[%d] --> Interruption du programme en cours...\n", getpid());
            getPostionOn = 0;
            receptionReseauEnCours = 0;
            receptionSerieEnCours = 0;
			break;
        default :
            printf (" Signal %d non traité \n", numSig );
            break ;
    }
}

void bye(){

    #ifndef PC
    destroyRobotMarvelmind(robotMarvelmind);
    closeSerialPort(portArduino);
    pthread_mutex_destroy(&mutexSerialPort);

    free(semMM);
    #endif

    closeReseauClient(sd);
    printf("Fin du programme\n");

}

int main(int argc, char const *argv[])
{
    DEBUG_PRINT("Debut du programme\n");
    // Installation du gestionnaire de fin de programme
    atexit(bye);

    
    // Installation du gestionnaire de signaux pour géré le ctrl c et la fin d'un fils.
    DEBUG_PRINT("Initialisation du gestionnaire de signaux\n");
    struct sigaction newAction;
    newAction.sa_handler = signalHandler;
    CHECK(sigemptyset(&newAction.sa_mask ), " sigemptyset ()");
    newAction.sa_flags = 0;
    CHECK(sigaction(SIGINT, &newAction, NULL), "sigaction (SIGINT)");

    #ifndef PC

    // Initialisation du port serie pour l'arduino
    DEBUG_PRINT("Ouverture du port serie pour l'arduino\n");
    
    CHECK(portArduino = openSerialPort(PORT_ARDUINO), "main: openSerialPort(PORT_ARDUINO)");
    setSerialPort(portArduino);

    DEBUG_PRINT("Initialisation de la mutex pour le port serie\n");
    CHECK_T(pthread_mutex_init(&mutexSerialPort, NULL), "main: pthread_mutex_init(&mutexSerialPort, NULL)");

     // Initialisation du marvelmind
    DEBUG_PRINT("Initialisation du marvelmind\n");
    CHECK_NULL(semMM = malloc(sizeof(sem_t)), "main: malloc(sizeof(sem_t))");
    robotMarvelmind = malloc(sizeof(struct RobotMarvelmind));
    initRobotMarvelmind(robotMarvelmind, PORT_MARVELMIND, ADDRESS_MM);

    #endif

   


    // Initialisation du reseau
    DEBUG_PRINT("Initialisation du reseau\n");
    initReseauClient(&sd, IP_SERVEUR);
    
    // Creation des threads
    pthread_t threadMarvelmind, threadReadArduino, threadReadReseau;

    DEBUG_PRINT("Creation du thread pour récupérer la position du marvelmind\n");
    CHECK_T(pthread_create(&threadMarvelmind, NULL, threadGetAndSendPositionMarvelmind, NULL), "main: pthread_create(&threadMarvelmind)");
    CHECK_T(pthread_create(&threadReadReseau, NULL, threadReceptionReseau, NULL), "main: pthread_create(&threadReadReseau)");

    #ifndef PC
    // CHECK_T(pthread_create(&threadReadArduino, NULL, threadReceptionSerie, NULL), "main: pthread_create(&threadReadArduino)");

    // pthread_join(threadReadArduino, NULL);
    #endif

    // Attente de la fin des thread
    pthread_join(threadMarvelmind, NULL);
    pthread_join(threadReadReseau, NULL);
   

    return 0;
}
