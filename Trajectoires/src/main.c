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

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();
        //testAvancer();
        if(deplacementSection(rbt,10) == 0)
        {
            printf("On est arrivé ! \n");
            clean();
            return 0;
        }
        sleep(0.5);
        compteur++; // compteur utilisé pour logger pas trop souvent
    }
    clean();
    return 0;
}