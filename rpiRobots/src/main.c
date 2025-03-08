/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - mainc.c                        */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                        S T A N D A R D   H E A D E R S                   */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <serial.h>
#include <main.h>
#include <marvelmindClient.h>
#include <networkClient.h>
#include <jansson.h>


/* ------------------------------------------------------------------------ */
/*                        G L O B A L   V A R I A B L E S                   */
/* ------------------------------------------------------------------------ */

int id = -1;
char ipServer[16];

int raspiOn = 1;
int marvelmindOn = 1; 

/* ------------------------------------------------------------------------ */
/*                    F U N C T I O N   D E F I N I T I O N                 */
/* ------------------------------------------------------------------------ */

void initParam(){
    json_t *root;
    json_error_t error;
    CHECK_NULL(root = json_load_file(PATH_PARAM, 0, &error), "initParam: json_load_file(PATH_PARAM)");
    id = json_integer_value(json_object_get(root, "id"));
    addressMM = json_integer_value(json_object_get(root, "addressMM"));
    if (raspiOn) strcpy(ipServer, json_string_value(json_object_get(root, "ipServer")));
    else strcpy(ipServer, "127.0.0.1");
    json_decref(root);
}


static void signalHandler(int numSig){ 
    switch(numSig) {
        case SIGINT : // handling SIGINT
            printf("\n\t[%d] --> Program interruption in progress...\n", getpid());
            getPositionON = 0;
            networkReceptionON = 0;
            serialReceptionON = 0;
            exit(EXIT_SUCCESS);
            break;
        default :
            printf (" Signal %d not handled \n", numSig);
            break ;
    }
}

void bye(){
    
    closeNetworkClient(sd);
    destroyRobotMarvelmind(robotMarvelmind);

    
    if (raspiOn){
        closeSerialPort(portArduino);
        pthread_mutex_destroy(&mutexSerialPort);
    }

    
    printf("End of program\n");

}

int main(int argc, char const *argv[])
{
    // Variables
    pthread_t threadMarvelmind,  threadReadReseau, threadReadArduino;


    DEBUG_PRINT("Start of program\n");

    // Installation of the program end handler
    atexit(bye);


    // Initialization of the execution mode
    if(argc > 1 && argv[1][0] == '0') marvelmindOn = 0;
    if(argc > 2 && argv[2][0] == '0') raspiOn = 0;
    

    DEBUG_PRINT("Execution mode: marvelmindOn=%d, raspiOn=%d\n", marvelmindOn, raspiOn);


    // Initialization of parameters
    initParam();

    
    // Installation of the signal handler to manage CTRL-C end
    DEBUG_PRINT("Initialization of the signal handler\n");
    struct sigaction newAction;
    newAction.sa_handler = signalHandler;
    CHECK(sigemptyset(&newAction.sa_mask ), " sigemptyset ()");
    newAction.sa_flags = 0;
    CHECK(sigaction(SIGINT, &newAction, NULL), "sigaction (SIGINT)");

    
    // Initialization of the serial port for the Arduino
    if (raspiOn){
        
        DEBUG_PRINT("Opening the serial port for the Arduino\n");
        CHECK(portArduino = openSerialPort(PORT_ARDUINO), "main: openSerialPort(PORT_ARDUINO)");
        setSerialPort(portArduino);

        DEBUG_PRINT("Initialization of the mutex for the serial port\n");
        CHECK_T(pthread_mutex_init(&mutexSerialPort, NULL), "main: pthread_mutex_init(&mutexSerialPort, NULL)");
    }
    

    // Initialization of the marvelmind
    DEBUG_PRINT("Initialization of the marvelmind\n");
    CHECK_NULL(semMM = malloc(sizeof(sem_t)), "main: malloc(sizeof(sem_t))");
    robotMarvelmind = malloc(sizeof(struct RobotMarvelmind));
    initRobotMarvelmind(robotMarvelmind, PORT_MARVELMIND, addressMM);


    // Initialization of the network
    DEBUG_PRINT("Initialization of the network on IP: %s\n", ipServer);
    initnetworkClient(&sd, ipServer);
    
    // Creation of threads
    DEBUG_PRINT("Creation of threads\n");
    CHECK_T(pthread_create(&threadMarvelmind, NULL, threadGetAndSendPositionMarvelmind, NULL), "main: pthread_create(&threadMarvelmind)");
    CHECK_T(pthread_create(&threadReadReseau, NULL, threadReceptionReseau, NULL), "main: pthread_create(&threadReadReseau)");

    if (raspiOn) CHECK_T(pthread_create(&threadReadArduino, NULL, threadReceptionSerie, NULL), "main: pthread_create(&threadReadArduino)");

    
    // Waiting for the end of threads 
    pthread_join(threadMarvelmind, NULL);
    pthread_join(threadReadReseau, NULL);
    if (raspiOn) pthread_join(threadReadArduino, NULL);
   

    return 0;
}
