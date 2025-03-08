#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <serial.h>
#include <main.h>
#include <marvelmindClient.h>
#include <reseauClient.h>
#include <jansson.h>


int id = -1;
char ipServeur[16];

int raspiOn = 1;
int marvelmindOn = 1; 

void initParam(){
    json_t *root;
    json_error_t error;
    CHECK_NULL(root = json_load_file(PATH_PARAM, 0, &error), "initParam: json_load_file(PATH_PARAM)");
    id = json_integer_value(json_object_get(root, "id"));
    addressMM = json_integer_value(json_object_get(root, "addressMM"));
    if (raspiOn) strcpy(ipServeur, json_string_value(json_object_get(root, "ipServeur")));
    else strcpy(ipServeur, "127.0.0.1");
    json_decref(root);
}


static void signalHandler(int numSig)
{ 
    switch(numSig) {
        case SIGINT : // traitement de SIGINT
            printf("\n\t[%d] --> Interruption du programme en cours...\n", getpid());
            getPostionOn = 0;
            receptionReseauEnCours = 0;
            receptionSerieEnCours = 0;
            exit(EXIT_SUCCESS);
			break;
        default :
            printf (" Signal %d non traité \n", numSig );
            break ;
    }
}

void bye(){
    
    closeReseauClient(sd);
    destroyRobotMarvelmind(robotMarvelmind);

    
    if (raspiOn){
        closeSerialPort(portArduino);
        pthread_mutex_destroy(&mutexSerialPort);
    }

    
    printf("Fin du programme\n");

}

int main(int argc, char const *argv[])
{
    DEBUG_PRINT("Debut du programme\n");

    // Installation du gestionnaire de fin de programme
    atexit(bye);

    // Initialisation du mode d'exécution

    if(argc > 2){
        if(argv[1][0] == '0') marvelmindOn = 0;
        if(argv[2][0] == '0') raspiOn = 0;
    }

    DEBUG_PRINT("Mode d'execution : marvelmindOn=%d, raspiOn=%d\n", marvelmindOn, raspiOn);

    // Initialisation des parametres
    initParam();

    
    // Installation du gestionnaire de signaux pour géré le ctrl c et la fin d'un fils.
    DEBUG_PRINT("Initialisation du gestionnaire de signaux\n");
    struct sigaction newAction;
    newAction.sa_handler = signalHandler;
    CHECK(sigemptyset(&newAction.sa_mask ), " sigemptyset ()");
    newAction.sa_flags = 0;
    CHECK(sigaction(SIGINT, &newAction, NULL), "sigaction (SIGINT)");

    
    // Initialisation du port serie pour l'arduino
    if (raspiOn){
        
        DEBUG_PRINT("Ouverture du port serie pour l'arduino\n");
        
        CHECK(portArduino = openSerialPort(PORT_ARDUINO), "main: openSerialPort(PORT_ARDUINO)");
        setSerialPort(portArduino);

        DEBUG_PRINT("Initialisation de la mutex pour le port serie\n");
        CHECK_T(pthread_mutex_init(&mutexSerialPort, NULL), "main: pthread_mutex_init(&mutexSerialPort, NULL)");
    }
    
    // Initialisation du marvelmind

    DEBUG_PRINT("Initialisation du marvelmind\n");
    CHECK_NULL(semMM = malloc(sizeof(sem_t)), "main: malloc(sizeof(sem_t))");
    robotMarvelmind = malloc(sizeof(struct RobotMarvelmind));
    initRobotMarvelmind(robotMarvelmind, PORT_MARVELMIND, addressMM);


    


    // Initialisation du reseau
    DEBUG_PRINT("Initialisation du reseau sur l'IP : %s\n", ipServeur);
    initReseauClient(&sd, ipServeur);
    
    // Creation des threads
    pthread_t threadMarvelmind,  threadReadReseau, threadReadArduino;

    DEBUG_PRINT("Creation des threads\n");
    CHECK_T(pthread_create(&threadMarvelmind, NULL, threadGetAndSendPositionMarvelmind, NULL), "main: pthread_create(&threadMarvelmind)");
    CHECK_T(pthread_create(&threadReadReseau, NULL, threadReceptionReseau, NULL), "main: pthread_create(&threadReadReseau)");

    if (raspiOn) CHECK_T(pthread_create(&threadReadArduino, NULL, threadReceptionSerie, NULL), "main: pthread_create(&threadReadArduino)");

    
    // Attente de la fin des threads 
    
    pthread_join(threadMarvelmind, NULL);
    pthread_join(threadReadReseau, NULL);
    if (raspiOn) pthread_join(threadReadArduino, NULL);
   

    return 0;
}
