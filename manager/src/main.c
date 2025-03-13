// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/main.c
#include <SFML/Graphics.h>
#include <robot.h>
#include <environnement.h>
#include <sections.h>
#include <utils.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include<utils.h>
#include<server.h>

#include<signal.h>
#include<bits/sigaction.h>
#include<bits/types/sigset_t.h>

#include <pthread.h>
#include <path.h>
#include <order.h>

static void signalHandler(int numSig){ 
    switch(numSig) {
        case SIGINT : // handling SIGINT
            printf("\n\t[%d] --> Program interruption in progress...\n", getpid());
            serverON = 0;
            exit(EXIT_SUCCESS);
            break;
        case SIGUSR1:
            DEBUG_PRINT("Signal SIGUSR1 received\n");
            newOrder();
            break;
        default :
            printf (" Signal %d not handled \n", numSig);
            break ;
    }
}

// Save the PID of the main process in a file for the other processes to use

void savePID()
{
    FILE *fpid;
    CHECK_NULL(fpid = fopen(PATH_FPID_MAIN, "w"), "fopen(fpid)"); 
    fprintf(fpid, "%d\n", getpid());                              
    fflush(fpid);                                                   
    fclose(fpid); 
}

void resetPID()
{
    FILE *fpid;
    CHECK_NULL(fpid = fopen(PATH_FPID_MAIN, "w"), "fopen(fpid)"); 
    fprintf(fpid, "-1\n");                              
    fflush(fpid);                                                   
    fclose(fpid); 
}


void bye()
{
    closeNetworkServer(se);
    freeRobots();
    printf("End of program\n");
    resetPID();
}



int main() 
{
    pthread_t tidPause, tidServer, tidStdin;

    atexit(bye); 
    
    // Initialization of the robots
    initRobots();

    // Save pid in a file
    savePID();

    // Installation of the signal handler to manage CTRL-C end
    DEBUG_PRINT("Initialization of the signal handler\n");
    struct sigaction newAction;
    newAction.sa_handler = signalHandler;
    CHECK(sigemptyset(&newAction.sa_mask ), " sigemptyset ()");
    newAction.sa_flags = 0;
    CHECK(sigaction(SIGINT, &newAction, NULL), "sigaction (SIGINT)");
    CHECK(sigaction(SIGUSR1, &newAction, NULL), "sigaction (SIGUSR1)");

    // Server
    initNetworkServer(&se);



    // mutex initialisation
    CHECK_T(pthread_mutex_init(&mutexOrdersElements, NULL), "main: pthread_mutex_init(&mutexOrdersElements, NULL)");





    // Initialisation des robots
    creer_cycle_principal();  
    creer_allees(); 
    creer_bacs(); 
    setupEnvironment();

    // processOrder("{{1:3}, {2:1}, {2:1}, {3:1}, {4:1}, {5:1}, {6:1}, {7:1}, {8:1}, {9:1}, {10:1}, {11:1}, {12:1}, {13:1}, {14:1}, {15:1}, {14:1}, {13:1}, {12:1}, {11:1}, {10:1}, {8:1}, {7:1}}");
    processOrder("{{1:1}, {2:1}, {2:1}, {3:1}, {4:1}, {5:1}, {6:1}, {7:1}, {8:1}, {9:1}, {10:1}, {11:1}, {12:1}, {13:1}, {14:1}, {15:1}, {14:1}, {13:1}, {12:1}, {11:1}, {10:1}, {8:1}, {7:1}}");
    
    printOrdersElementary();

    // Initialisation des threads
    // Create threads
    CHECK(pthread_create(&tidPause, NULL, threadWaitOrder, NULL), "pthread_create(tidPause)");
    CHECK(pthread_create(&tidServer, NULL, threadServer, NULL), "pthread_create(tidServer)");
    CHECK(pthread_create(&tidStdin, NULL, testEnvoiStdin, NULL), "pthread_create(tidStdin)");

    // Créer les threads pour chaque robot
    pthread_t tidRobot1, tidRobot2, tidRobot3;
    pthread_create(&tidRobot1, NULL, update_robot, (void*)rbt);
    pthread_create(&tidRobot2, NULL, update_robot, (void*)rbt2);
    pthread_create(&tidRobot3, NULL, update_robot, (void*)rbt3);

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();
        sleep(0.5);
        compteur++; // compteur utilisé pour logger pas trop souvent
    }

    pthread_join(tidRobot1, NULL);
    pthread_join(tidRobot2, NULL);
    pthread_join(tidRobot3, NULL);

    pthread_join(tidPause, NULL);
    pthread_join(tidServer, NULL);
    pthread_join(tidStdin, NULL);

    clean();
    return 0;
}
