// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/main.c
#include <SFML/Graphics.h>
#include <robot.h>
#include <environnement.h>
#include <sections.h>
#include <utils.h>
#include <stdio.h>
#include <unistd.h>

int main() 
{
    creer_cycle_principal();    
    setupEnvironment();
    //pthread_mutex_lock(&s_principale[3]->mutex);

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();
        //testAvancer();

        if(deplacementSection(rbt2,0) == 0)
        {
            printf("On est arrivé robot 2 ! \n");
            //clean();
            //return 0;
        }
        if(deplacementSection(rbt,9) == 0)
        {
            printf("On est arrivé robot 1 ! \n");
            //clean();
            //return 0;
        }

        if(deplacementSection(rbt3,0) == 0)
        {
            printf("On est arrivé robot 3 ! \n");
            //clean();
            //return 0;
        }
        sleep(0.5);
        compteur++; // compteur utilisé pour logger pas trop souvent
    }
    clean();
    return 0;
}