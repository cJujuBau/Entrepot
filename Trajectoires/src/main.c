// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/main.c
#include <SFML/Graphics.h>
#include <robot.h>
#include <environnement.h>
#include <sections.h>
#include <utils.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

sfVector2f* cheminObjet1;
ItemPath* objet1;
sfVector2f* chemin1Objet2;
sfVector2f* chemin2Objet2;
ItemPath* objet2;

//Fonction de mise à jour pour chaque robot
void* update_robot(void* arg) {
    robot* rbt = (robot*)arg;
    ItemPath* o1 = malloc(sizeof(ItemPath));
    ItemPath* o2 = malloc(sizeof(ItemPath));
    //ItemPath* objet = (rbt == rbt3) ? objet1 : objet2;

    // o1 est le premier robot à aller chercher ( allée la lus proche )
    // o2 est le deuxième robot à aller chercher ( allée la plus éloignée )
    if(objet1->rack > objet2->rack)
    {
        o1 = objet2;
        o2 = objet1;
    }
    else
    {
        o1 = objet1;
        o2 = objet2;
    }

    while (1) {
        if (rbt->etape == etapeDeplacementAvantCollecte) {
            if (deplacementSection(rbt, 0) == 0) {
                rbt->etape = etapeCollecte;
            }
        } else if (rbt->etape == etapeCollecte) {
            if (chercheObjet(rbt, o1) == 0) {
                printf("Robot %d a trouvé l'objet 1 ! \n", rbt->id_robot);
                rbt->etape = etapeRetourCyclePrincipal;
            }
        } 
        else if (rbt->etape == etapeRetourCyclePrincipal) {
            if (retourCyclePrincipal(rbt) == 0) {
                int numero_section_entree = 1 + 2 * (o2->aisleL - 1);
                // si le robot peut encore aller chercher son euxième objet
                if(numero_section_entree >= rbt->numero_section)
                {
                    printf("Le robot va aller chercher l'objet 2 \n");
                    rbt->etape = etapeCollecte2;
                }
                else
                {
                    printf("Robot %d est de retour sur le cycle principal ! \n", rbt->id_robot);
                    rbt->etape = etapeDeplacementAvantDepose;
                }
            }
        } 
        else if(rbt->etape == etapeCollecte2)
        {
            if (chercheObjet(rbt, o2) == 0) {
                printf("Robot %d a trouvé l'objet 2 ! \n", rbt->id_robot);
                rbt->etape = etapeRetourCyclePrincipal2;
            }
        }
        else if(rbt->etape == etapeRetourCyclePrincipal2)
        {
            if (retourCyclePrincipal(rbt) == 0) 
            {
                printf("Robot %d est de retour sur le cycle principal ! \n", rbt->id_robot);
                rbt->etape = etapeDeplacementAvantDepose;
            }
        }
        else if (rbt->etape == etapeDeplacementAvantDepose) {
            int section_avant_bac = NOMBRE_SECTIONS_PRINCIPALES - 4 / objet1->bac;
            if (deplacementSection(rbt, section_avant_bac) == 0) {
                printf("Robot %d est devant le bac %d ! \n", rbt->id_robot, o1->bac);
                rbt->etape = etapeDepose;
            }
        } else if (rbt->etape == etapeDepose) {
            if (deposeBac(rbt, o1->bac) == 0) {
                printf("Robot %d a déposé l'objet ! \n", rbt->id_robot);
                rbt->etape = etapeDeplacementAvantCollecte;
            }
        }
        sleep(0.5);
    }
    return NULL;
}

int main() 
{
    creer_cycle_principal();  
    creer_allees(); 
    creer_bacs(); 
    setupEnvironment();

    cheminObjet1 = malloc(2*sizeof(sfVector2f));
    cheminObjet1[0] = (sfVector2f) {LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - LARGEUR_ETAGERE - LARGEUR_ALLEE / 2, 5 * LONGUEUR_ETAGERE / 8};
    cheminObjet1[1] = (sfVector2f) {LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - LARGEUR_ETAGERE, 5 * LONGUEUR_ETAGERE / 8};
    objet1 = malloc(sizeof(ItemPath));
    //*objet1 = (ItemPath) {0, 1, 1, 2, 1, 1, -1, cheminObjet1, NULL};

    chemin1Objet2 = malloc(2*sizeof(sfVector2f));
    chemin1Objet2[0] = (sfVector2f) {ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE / 2, LONGUEUR_ETAGERE / 8};
    chemin1Objet2[1] = (sfVector2f) {ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE , LONGUEUR_ETAGERE / 8};

    chemin2Objet2 = malloc(2*sizeof(sfVector2f));
    chemin2Objet2[0] = (sfVector2f) {ECART_LONGUEUR + 2*LARGEUR_BAC + 3*LARGEUR_ALLEE / 2, LONGUEUR_ETAGERE / 8};
    chemin2Objet2[1] = (sfVector2f) {ECART_LONGUEUR + 2*LARGEUR_BAC + LARGEUR_ALLEE , LONGUEUR_ETAGERE / 8};
    objet2 = malloc(sizeof(ItemPath));

    *objet1 = (ItemPath) {1, 2, 2, 4, 1, 3, 2, chemin1Objet2, chemin2Objet2};
    *objet2 = (ItemPath) {1, 2, 2, 4, 1, 3, 2, chemin1Objet2, chemin2Objet2};

    // Créer les threads pour chaque robot
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, update_robot, (void*)rbt);
    pthread_create(&thread2, NULL, update_robot, (void*)rbt2);
    pthread_create(&thread3, NULL, update_robot, (void*)rbt3);

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();
        sleep(0.5);
        compteur++; // compteur utilisé pour logger pas trop souvent
    }

    // Attendre la fin des threads (optionnel, si vous voulez que le programme attende la fin des threads avant de se terminer)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    clean();
    return 0;
}
