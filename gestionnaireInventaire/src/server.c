#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include <server.h>

#define MAX_ROBOTS 5

#define MAXCAR 100
#define LOCALPORT 3000


int receptionON = 1;
int serverON = 1;
int se = -1;

typedef struct RobotPosition{
    int id, sd;
    int x, y;
    int xRef, yRef, oRef;
} *RobotPosition;

RobotPosition robotsPositions[MAX_ROBOTS];

void *threadConnexionClient(void *arg)
{
    static int nbClient = 0;
    int sd = *(int *)arg;
    free(arg);

    int idClient = ++nbClient;
    char buff[MAXCAR + 1];
    int nbChar;

    
    char indent[5];
    for (int i = 0; i < idClient; i++) indent[i] = '\t';
    indent[idClient] = '\0';

    DEBUG_PRINT("%sThread launched for client %d on file descriptor %d\n", indent, idClient, sd);
    while (receptionON)
    {
        nbChar = recv(sd, buff, MAXCAR, 0);

        if (nbChar > 0)
        {
            buff[nbChar] = '\0';
            DEBUG_PRINT("%s%d received : '%s'\n", indent, idClient, buff);
            processStringReceived(buff, nbChar, sd); // Voir si n met un check
        }
        else if (nbChar == 0)
        {
            DEBUG_PRINT("%sClient %d closed the connection\n", indent, idClient);
            break;
        }
        else
        {
            // If the error is due to an interruption, continue waiting
            if (errno == EINTR) continue;
            else {
                perror("threadConnexionClient: recv - Reception error");
                break;
            }
        }
    }

    close(sd);
    DEBUG_PRINT("%sthreadServer: Thread finished for client %d on file descriptor %d\n", indent, idClient, sd);
    DEBUG_PRINT("\n###############################\n");
    DEBUG_PRINT("Remaining number of clients connected: %d\n", --nbClient);
    DEBUG_PRINT("\n###############################\n");

    pthread_exit(NULL);
    
    return NULL;
}

void  initNetworkServer(int *se)
{
    CHECK(*se = socket(AF_INET, SOCK_STREAM, 0), "Socket creation error");

    int opt = 1;
    CHECK(setsockopt(*se, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "setsockopt");

    struct sockaddr_in addrserv;
    addrserv.sin_family = AF_INET;
    addrserv.sin_addr.s_addr = INADDR_ANY;
    addrserv.sin_port = htons(LOCALPORT);

    CHECK(bind(*se, (struct sockaddr *)&addrserv, sizeof(addrserv)), "Bind error");
    listen(*se, 5);

    printf("\n\tTCP SERVER listening on port %d\n\n", LOCALPORT);
}

void closeNetworkServer(int se)
{
    close(se);
}

void *threadServer(void *arg)
{
    int *sd;
    pthread_t tid;

    while (serverON)
    {
        sd = malloc(sizeof(int));
        *sd = accept(se, NULL, NULL);
        CHECK(*sd, "accept");
        DEBUG_PRINT("\n###############################\n");
        DEBUG_PRINT("New connection accepted %d\n", *sd);

        if (pthread_create(&tid, NULL, threadConnexionClient, sd) != 0)
        {
            perror("pthread_create");
            close(*sd);
            free(sd);
        }

        pthread_detach(tid); // Automatically release thread resources
    }

    pthread_exit(NULL);
    
    return NULL;
}

int processStringReceived(const char *buffer, const int size, int sd)
{
    int idRobot;
    char commande;
    char args[size-3];
    sscanf(buffer, "%d-%c:%s", &idRobot, &commande, args);
    DEBUG_PRINT("idRobot=%d, commande=%c, args=%s\n", idRobot, commande, args);
    if (commande == 'p') {
        sscanf(args, "%d,%d;", &robotsPositions[idRobot]->x, &robotsPositions[idRobot]->y);
        sendPosRef(idRobot);
    }
    if (commande == 'i') robotsPositions[idRobot]->sd = sd;
    if (commande == 'o') {
        int x, y, o;
        sscanf(args, "%d,%d,%d;", &x, &y, &o);
        obstacleDetected(x, y, o);
    }
    
    printRobot(idRobot);
    return 0;
}

void obstacleDetected(int x, int y, int o)
{
    printf("Obstacle detected at x=%d, y=%d, o=%d\n", x, y, o);
}

int sendPosRef(int id) // peut etre envoyer un robotPosition
{
    char buffer[25];
    sprintf(buffer, "r:%d,%d,%d;", robotsPositions[id]->xRef, robotsPositions[id]->yRef, robotsPositions[id]->oRef);
    DEBUG_PRINT("sendPosRef: buffer=%s\n", buffer);
    return send(robotsPositions[id]->sd, buffer, strlen(buffer), 0);
}




void initRobots()
{
    for (int i = 0; i < MAX_ROBOTS; i++)
    {
        CHECK_NULL(robotsPositions[i] = malloc(sizeof(struct RobotPosition)), "initRobots: malloc(RobotPosition)");

        robotsPositions[i]->id = i;
        robotsPositions[i]->sd = -1;
        robotsPositions[i]->x = 0;
        robotsPositions[i]->y = 0;
        robotsPositions[i]->xRef = rand() % 1000;
        robotsPositions[i]->yRef = rand() % 1000;
        robotsPositions[i]->oRef = rand() % 360;
    }
}

void freeRobots()
{
    for (int i = 0; i < MAX_ROBOTS; i++) free(robotsPositions[i]);
}

void printRobots()
{
    for (int i = 0; i < MAX_ROBOTS; i++) printRobot(i);
}

void printRobot(int i)
{
    DEBUG_PRINT("Robot %d : sd=%d, x=%d, y=%d, xRef=%d, yRef=%d, oRef=%d\n", robotsPositions[i]->id, robotsPositions[i]->sd, robotsPositions[i]->x, robotsPositions[i]->y, robotsPositions[i]->xRef, robotsPositions[i]->yRef, robotsPositions[i]->oRef);
}


