/****************************************************/
/* Application : serveurTCP                            */
/* Date : 13/01/2023                                */
/* Version : 1                                      */
/* Compilation : gcc serveurTCP.c -o serveurTCP     */
/* Usage : ./serveurUdp  [port]                     */
/****************************************************/

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <string.h>
// #include <unistd.h>
#include <errno.h>


// #define MAXCAR  80
// #define LOCALIP "127.0.0.1"
// #define LOCALPORT 3000
 
// #define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(1);}

// int main(int argc, char *argv[]) {
//     int se, sd;
//     struct sockaddr_in addrserv;
//     int opt = 1; // Add this line

//     int nbcar;
//     char buff[MAXCAR + 1];

//     // Etape 1 - Creation de la socket
//     se = socket(AF_INET, SOCK_STREAM, 0);
//     CHECKERROR(se, -1, "Creation fail !!!\n");

//     // Add these lines:
//     if (setsockopt(se, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
//         perror("setsockopt");
//         exit(1);
//     }

//     // Etape 2 - Adressage de la socket d'ecoute
//     addrserv.sin_family = AF_INET;
//     addrserv.sin_addr.s_addr = INADDR_ANY;
//     addrserv.sin_port = htons(LOCALPORT);

//     // Etape 3 - Affectation de l'adresse a la socket avec controle d'erreur
//     CHECKERROR(bind(se, (const struct sockaddr *)&addrserv, sizeof(addrserv)), -1, "Erreur de bind !!!\n");

//     // Etape 4 - definir le nombre de connexions simultannées
//     listen(se, 1);

//     // Etape 5 - mise du serveur a l'ecoute des demandes de connexion
//     printf("\n\tSERVEUR TCP\n\n");

//     while (1)
//     {
//         sd = accept(se, NULL, NULL);
//         CHECKERROR(sd, -1, "L'ouverture de connexion n'a pas fonctionne !!!\n");

//         printf("Ouverture d'une nouvelle connexion !!! \n");
        
//         while (1)
//         {
//             nbcar = recv(sd, buff, MAXCAR + 1, 0);
//             printf("nbcar = %d\n", nbcar);


//             if (nbcar > 0) {
//                 printf("SERVEUR recu : '%s'\n", buff);
                
//                 // Envoyer le message reçu au client
//                 send(sd, "Je te derange", 13, 0);


//             }  else if (nbcar == 0) {
//                 printf("Client a fermé la connexion\n");
//                 close(sd); // Fermer la connexion si le client a fermé la connexion
//                 break; // Sortir de la boucle de traitement des messages
//             } else {
//                 if (errno == EINTR) {
//                     // Si l'erreur est due à une interruption, continuer à attendre
//                     continue;
//                 } else {
//                     printf("Ca ne fonctionne pas !!!\n");
//                     close(sd); // Fermer la connexion si recv échoue pour une autre raison
//                     break; // Sortir de la boucle de traitement des messages
//                 }
//             }
            
          
//         }
        
        

//         printf("Arret de la connexion !!! \n");
//         printf("\n###############################\n");
//     }


// return EXIT_SUCCESS;
// }


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAXCAR 100
#define LOCALPORT 3000

#define CHECKERROR(var, val, msg) if (var == val) { perror(msg); exit(1); }

void *threadServeur(void *arg) {
    int sd = *(int *) arg;
    free(arg);

    char buff[MAXCAR + 1];
    int nbChar;

    printf("Thread lancé pour le client %d\n", sd);

    while (1) {
        nbChar = recv(sd, buff, MAXCAR, 0);

        if (nbChar > 0) {
            buff[nbChar] = '\0';
            printf("SERVEUR reçu de %d : '%s'\n", sd, buff);
            
        } else if (nbChar == 0) {
            printf("Client %d a fermé la connexion\n", sd);
            break;
        } else {
            if (errno == EINTR) {
                // Si l'erreur est due à une interruption, continuer à attendre
                continue;
            } else {
                perror("threadServeur: recv");
                printf("Ca ne fonctionne pas !!!\n");
                close(sd); // Fermer la connexion si recv échoue pour une autre raison
                break; // Sortir de la boucle de traitement des messages
            }
        }
    }

    close(sd);
    printf("threadServeur: Thread fini pour le client %d\n", sd);
    return NULL;
}

int main() {
    int se, *sd;
    struct sockaddr_in addrserv;
    pthread_t tid;

    se = socket(AF_INET, SOCK_STREAM, 0);
    CHECKERROR(se, -1, "Erreur de creation socket");

    int opt = 1;
    CHECKERROR(setsockopt(se, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), -1, "setsockopt");

    addrserv.sin_family = AF_INET;
    addrserv.sin_addr.s_addr = INADDR_ANY;
    addrserv.sin_port = htons(LOCALPORT);

    CHECKERROR(bind(se, (struct sockaddr*)&addrserv, sizeof(addrserv)), -1, "Erreur de bind");
    listen(se, 5);
    printf("SERVEUR TCP en écoute sur le port %d\n", LOCALPORT);

    while (1) {
        sd = malloc(sizeof(int));
        *sd = accept(se, NULL, NULL);
        CHECKERROR(*sd, -1, "accept");
        printf("Nouvelle connexion acceptée %d\n", *sd);

        if (pthread_create(&tid, NULL, threadServeur, sd) != 0) {
            perror("pthread_create");
            close(*sd);
            free(sd);
        }
        pthread_detach(tid); // Permet de libérer automatiquement les ressources du thread
    }

    close(se);
    return EXIT_SUCCESS;
}


    
    
