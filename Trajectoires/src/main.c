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
    creer_bacs(); 
    setupEnvironment();

    sfVector2f* cheminObjet1 = malloc(2*sizeof(sfVector2f));
    cheminObjet1[0] = (sfVector2f) {LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - LARGEUR_ETAGERE - LARGEUR_ALLEE / 2, 5 * LONGUEUR_ETAGERE / 8};
    cheminObjet1[1] = (sfVector2f) {LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - LARGEUR_ETAGERE, 5 * LONGUEUR_ETAGERE / 8};
    ItemPath objet1 = {0, 1, 1, 2, 1, 1, -1, cheminObjet1, NULL};

    sfVector2f* chemin1Objet2 = malloc(2*sizeof(sfVector2f));
    chemin1Objet2[0] = (sfVector2f) {ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE / 2, LONGUEUR_ETAGERE / 8};
    chemin1Objet2[1] = (sfVector2f) {ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE , LONGUEUR_ETAGERE / 8};

    sfVector2f* chemin2Objet2 = malloc(2*sizeof(sfVector2f));
    chemin2Objet2[0] = (sfVector2f) {ECART_LONGUEUR + 2*LARGEUR_BAC + 3*LARGEUR_ALLEE / 2, LONGUEUR_ETAGERE / 8};
    chemin2Objet2[1] = (sfVector2f) {ECART_LONGUEUR + 2*LARGEUR_BAC + LARGEUR_ALLEE , LONGUEUR_ETAGERE / 8};
    ItemPath objet2 = {1, 2, 2, 4, 1, 3, 2, chemin1Objet2, chemin2Objet2};

    //pthread_mutex_lock(&allee_etageres[0]->mutex);

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();

        if(rbt->etape == etapeDeplacementAvantCollecte)
        {
            if(deplacementSection(rbt,0) == 0)
            {
                rbt->etape = etapeCollecte;
            }
        }
        else if(rbt->etape == etapeCollecte)
        {
            if(chercheObjet(rbt,objet2) == 0)
            {
                printf("Rbt1 a trouvé l'objet 2 ! \n");
                rbt->etape = etapeRetourCyclePrincipal;
            }
        }
        else if(rbt->etape == etapeRetourCyclePrincipal)
        {
            if(retourCyclePrincipal(rbt) == 0)
            {
               printf("Rbt1 est de retour sur le cycle principal ! \n");
               rbt->etape = etapeDelacementAvantDepose;
            }
        }
        else if(rbt->etape == etapeDelacementAvantDepose)
        {
            int section_avant_bac = NOMBRE_SECTIONS_PRINCIPALES - 4 / objet2.bac;
            //printf("Le robot 1 doit se rendre à la section %d \n", section_avant_bac);
            if(deplacementSection(rbt,section_avant_bac) == 0)
            {
                printf("Rbt1 est devant le bac %d ! \n", objet2.bac);
                rbt->etape = etapeDepose;
            }
        }
        else if(rbt->etape == etapeDepose)
        {
            if(deposeBac(rbt,objet2.bac) == 0)
            {
                printf("Rbt1 a déposé l'objet 2 ! \n");
                rbt->etape = etapeDeplacementAvantCollecte;
            }
        }

        if(rbt2->etape == etapeDeplacementAvantCollecte)
        {
            if(deplacementSection(rbt2,0) == 0)
            {
                rbt2->etape = etapeCollecte;
            }
        }
        else if(rbt2->etape == etapeCollecte)
        {
            if(chercheObjet(rbt2,objet2) == 0)
            {
                printf("Rbt2 a trouvé l'objet 2 ! \n");
                rbt2->etape = etapeRetourCyclePrincipal;
            }
        }
        else if(rbt2->etape == etapeRetourCyclePrincipal)
        {
            if(retourCyclePrincipal(rbt2) == 0)
            {
               printf("Rbt2 est de retour sur le cycle principal ! \n");
               rbt2->etape = etapeDelacementAvantDepose;
            }
        }
        else if(rbt2->etape == etapeDelacementAvantDepose)
        {
            int section_avant_bac = NOMBRE_SECTIONS_PRINCIPALES - 4 / objet2.bac;
            //printf("Le robot 2 doit se rendre à la section %d \n", section_avant_bac);
            if(deplacementSection(rbt2,section_avant_bac) == 0)
            {
                printf("Rbt2 est devant le bac %d ! \n", objet2.bac);
                rbt2->etape = etapeDepose;
            }
        }
        else if(rbt2->etape == etapeDepose)
        {
            if(deposeBac(rbt2,objet2.bac) == 0)
            {
                printf("Rbt2 a déposé l'objet 2 ! \n");
                rbt2->etape = etapeDeplacementAvantCollecte;
            }
        }

        if(rbt3->etape == etapeDeplacementAvantCollecte)
        {
            if(deplacementSection(rbt3,0) == 0)
            {
                rbt3->etape = etapeCollecte;
            }
        }
        else if(rbt3->etape == etapeCollecte)
        {
            if(chercheObjet(rbt3,objet1) == 0)
            {
                printf("Rbt3 a trouvé l'objet 1 ! \n");
                rbt3->etape = etapeRetourCyclePrincipal;
            }
        }
        else if(rbt3->etape == etapeRetourCyclePrincipal)
        {
            if(retourCyclePrincipal(rbt3) == 0)
            {
               printf("Rbt3 est de retour sur le cycle principal ! \n");
               rbt3->etape = etapeDelacementAvantDepose;
            }
        }
        else if(rbt3->etape == etapeDelacementAvantDepose)
        {
            int section_avant_bac = NOMBRE_SECTIONS_PRINCIPALES - 4 / objet1.bac;
            //printf("Le robot 3 doit se rendre à la section %d \n", section_avant_bac);
            if(deplacementSection(rbt3,section_avant_bac) == 0)
            {
                printf("Rbt3 est devant le bac %d ! \n", objet2.bac);
                rbt3->etape = etapeDepose;
            }
        }
        else if(rbt3->etape == etapeDepose)
        {
            if(deposeBac(rbt3,objet1.bac) == 0)
            {
                printf("Rbt3 a déposé l'objet 1 ! \n");
                rbt3->etape = etapeDeplacementAvantCollecte;
            }
        }

        sleep(0.5);
        compteur++; // compteur utilisé pour logger pas trop souvent
    }
    clean();
    return 0;
}