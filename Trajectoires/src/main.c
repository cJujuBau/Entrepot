// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/main.c
#include <SFML/Graphics.h>
#include <robot.h>
#include <environnement.h>
#include <sections.h>
#include <utils.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() 
{
    creer_cycle_principal();  
    creer_allees();  
    setupEnvironment();

    sfVector2f* cheminObjet1 = malloc(2*sizeof(sfVector2f));
    cheminObjet1[0] = (sfVector2f) {LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - LARGEUR_ETAGERE - LARGEUR_ALLEE / 2, 5 * LONGUEUR_ETAGERE / 8};
    cheminObjet1[1] = (sfVector2f) {LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - LARGEUR_ETAGERE, 5 * LONGUEUR_ETAGERE / 8};
    ItemPath objet1 = {0, 1, 2, 1, 1, -1, cheminObjet1, NULL};

    sfVector2f* chemin1Objet2 = malloc(2*sizeof(sfVector2f));
    chemin1Objet2[0] = (sfVector2f) {ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE / 2, LONGUEUR_ETAGERE / 8};
    chemin1Objet2[1] = (sfVector2f) {ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE , LONGUEUR_ETAGERE / 8};

    sfVector2f* chemin2Objet2 = malloc(2*sizeof(sfVector2f));
    chemin2Objet2[0] = (sfVector2f) {ECART_LONGUEUR + 2*LARGEUR_BAC + 3*LARGEUR_ALLEE / 2, LONGUEUR_ETAGERE / 8};
    chemin2Objet2[1] = (sfVector2f) {ECART_LONGUEUR + 2*LARGEUR_BAC + LARGEUR_ALLEE , LONGUEUR_ETAGERE / 8};
    ItemPath objet2 = {1, 2, 4, 1, 3, 2, chemin1Objet2, chemin2Objet2};

    //pthread_mutex_lock(&allee_etageres[0]->mutex);

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();
        //testAvancer();

        // if(deplacementSection(rbt2,2) == 0)
        // {
        //     printf("On est arrivé robot 2 ! \n");
        //     //clean();
        //     //return 0;
        // }

        // if(deplacementSection(rbt,9) == 0)
        // {
        //     printf("On est arrivé robot 1 ! \n");
        //     //clean();
        //     //return 0;

        // }

        if(chercheObjet(1,rbt,objet1) == 0)
        {
            printf("Rbt1 a trouvé l'objet ! \n");
            //clean();
            //return 0;
        }
        if(chercheObjet(2,rbt2,objet2) == 0)
        {
            printf("Rbt2 a trouvé l'objet ! \n");
            //clean();
            //return 0;
        }
        if(chercheObjet(3,rbt3,objet2) == 0)
        {
            printf("Rbt3 a trouvé l'objet ! \n");
            //clean();
            //return 0;
        }

        // if(deplacementSection(rbt3,0) == 0)
        // {
        //     printf("On est arrivé robot 3 ! \n");
        //     //clean();
        //     //return 0;
        // }
        sleep(0.5);
        compteur++; // compteur utilisé pour logger pas trop souvent
    }
    clean();
    return 0;
}