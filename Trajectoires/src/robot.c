// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/robot.c
#include <robot.h>
#include <environnement.h>
#include <sections.h>
#include <utils.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/Graphics.h>

robot* rbt = NULL; // Définition de la variable externe

extern section_cycle_principal* s_principale[];

void creer_robot(robot** r, int n_section, int n_wayPoint) {
    *r = malloc(sizeof(robot));
    if (*r == NULL) {
        perror("Erreur malloc robot");
        exit(EXIT_FAILURE);
    }

    (*r)->pos = malloc(sizeof(sfVector2f));
    if ((*r)->pos == NULL) {
        perror("Erreur malloc pos");
        free(*r);
        exit(EXIT_FAILURE);
    }
    rbt->cercle = sfCircleShape_create();
    actualiseSectionWayPointRobot(rbt,n_section,n_wayPoint);
    sfVector2f pos = s_principale[n_section]->point_section[n_wayPoint];
    actualisePositionRobot(rbt,pos);
    sfCircleShape_setRadius(rbt->cercle, 15);
    sfCircleShape_setFillColor(rbt->cercle,sfRed);
}

void actualisePositionRobot(robot* rbt, sfVector2f nouvellePosition)
{
    (rbt->pos)->x = nouvellePosition.x;
    (rbt->pos)->y = nouvellePosition.y;
    sfCircleShape_setPosition(rbt->cercle,nouvellePosition);
}

void actualiseSectionWayPointRobot(robot* rbt, int n_section, int n_wayPoint)
{
    rbt->numero_section = n_section;
    rbt->numero_wayPoint = n_wayPoint;
}


int Deplacement_elementaire(robot* rbt, sfVector2f posfinale)
{
    sfVector2f posRobot = *(rbt->pos);
    float d = distance(posRobot,posfinale);
    if(d > 5)
    {
        sfVector2f nouvellePosition = {(rbt->pos)->x + VITESSE_ROBOT * ((posfinale.x - posRobot.x)/d), (rbt->pos)->y + VITESSE_ROBOT * ((posfinale.y - posRobot.y)/d)}; 
        if(compteur  % 100 == 0)
        printf("Nouvelle position du robot : (%f,%f) \n", nouvellePosition.x, nouvellePosition.y);
        actualisePositionRobot(rbt,nouvellePosition);
        return 1;
    }
    return 0;
}

int deplacementSection(robot* rbt, int numero_section_objectif)
{
    int resultat_deplacement;
    int indice_prochaine_section;
    int indice_prochain_wayPoint;
    
    if(rbt->numero_wayPoint < (s_principale[(rbt->numero_section)]->nombre_points)-1)
    {
        indice_prochaine_section = rbt->numero_section;
        indice_prochain_wayPoint = rbt->numero_wayPoint + 1;
    }
    else
    {
        if(rbt->numero_section == numero_section_objectif)
        {
            return 0;
        }
        else
        {
            indice_prochaine_section = ((rbt->numero_section) + 1) % NOMBRE_SECTIONS_PRINCIPALES;
            indice_prochain_wayPoint = 0;
        }
    }

    resultat_deplacement = Deplacement_elementaire(rbt,s_principale[indice_prochaine_section]->point_section[indice_prochain_wayPoint]);
    if(resultat_deplacement == 0)
    {
        actualiseSectionWayPointRobot(rbt,indice_prochaine_section,indice_prochain_wayPoint);
    }
    return 1;
}

void testAvancer()
{
    sfVector2f pos = {0,0};
    Deplacement_elementaire(rbt,pos);
}