#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

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

    // Create threads
    CHECK(pthread_create(&tidPause, NULL, threadWaitOrder, NULL), "pthread_create(tidPause)");
    CHECK(pthread_create(&tidServer, NULL, threadServer, NULL), "pthread_create(tidServer)");
    CHECK(pthread_create(&tidStdin, NULL, testEnvoiStdin, NULL), "pthread_create(tidStdin)");

    
    // Waiting for threads to finish
    pthread_join(tidPause, NULL);
    pthread_join(tidServer, NULL);
    pthread_join(tidStdin, NULL);
    
    return EXIT_SUCCESS;
}