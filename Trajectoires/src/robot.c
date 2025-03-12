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
#include <pthread.h>

robot* rbt = NULL; // Définition de la variable externe
robot* rbt2 = NULL;
robot* rbt3 = NULL;

extern section_cycle_principal* s_principale[];
extern Allee* allee_etageres[];

void creer_robot(robot** r, int n_section, int n_wayPoint) {
    *r = malloc(sizeof(robot));
    if (*r == NULL) {
        perror("Erreur malloc robot");
        free(*r);
        exit(EXIT_FAILURE);
    }

    (*r)->pos = malloc(sizeof(sfVector2f));
    if ((*r)->pos == NULL) {
        perror("Erreur malloc pos");
        free((*r)->pos);
        free(*r);
        exit(EXIT_FAILURE);
    }
    (*r)->cercle = sfCircleShape_create();
    actualiseSectionWayPointRobot(*r,n_section,n_wayPoint);
    sfVector2f pos = s_principale[n_section]->point_section[n_wayPoint];
    actualisePositionRobot(*r,pos);
    pthread_mutex_lock(&s_principale[n_section]->mutex);
    sfCircleShape_setRadius((*r)->cercle, 15);
    sfCircleShape_setFillColor((*r)->cercle,sfRed);
    (*r)->hasMutex = 0;
    (*r)->prochaineAllee = -1; // pas d'allée choisie
    (*r)->isInAisle = 0; // pas encore dans l'allée
    (*r)->cheminAllee = malloc(2*sizeof(sfVector2f));
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

// déplace légèrement le robot vers la position finale
// si la distance entre la position actuelle et la position finale est supérieure à 5
// sinon, il ne fait rien
int Deplacement_elementaire(robot* rbt, sfVector2f posfinale)
{
    sfVector2f posRobot = *(rbt->pos);
    float d = distance(posRobot,posfinale);
    if(d > 5)
    {
        sfVector2f nouvellePosition = {(rbt->pos)->x + VITESSE_ROBOT * ((posfinale.x - posRobot.x)/d), (rbt->pos)->y + VITESSE_ROBOT * ((posfinale.y - posRobot.y)/d)}; 
        if(compteur  % 100 == 0)
        //printf("Nouvelle position du robot : (%f,%f) \n", nouvellePosition.x, nouvellePosition.y);
        actualisePositionRobot(rbt,nouvellePosition);
        return 1;
    }
    return 0;
}

int deplacementSection(robot* rbt, int numero_section_objectif)
{
    //printf("On veut se rendre à la section %d \n", numero_section_objectif);
    //printf("On est à la section %d et au wayPoint %d \n", rbt->numero_section, rbt->numero_wayPoint);

    int resultat_deplacement;
    int indice_prochaine_section;
    int indice_prochain_wayPoint;
    int derniere_section = rbt->numero_section; // Variable pour suivre la dernière section
    
    // Si on n'est pas au dernier wayPoint de la section
    if(rbt->numero_wayPoint < (s_principale[(rbt->numero_section)]->nombre_points)-1)
    {
        //printf("On se déplace vers la fin de la section \n");
        indice_prochaine_section = rbt->numero_section;
        indice_prochain_wayPoint = rbt->numero_wayPoint + 1;
        //printf("Prochaine section : %d / prochain waypoint : %d \n", indice_prochaine_section, indice_prochain_wayPoint);
        resultat_deplacement = Deplacement_elementaire(rbt, s_principale[indice_prochaine_section]->point_section[indice_prochain_wayPoint]);
        
        if (resultat_deplacement == 0)
        {
            actualiseSectionWayPointRobot(rbt, indice_prochaine_section, indice_prochain_wayPoint);
        }
    }
    else
    {
        //printf("On est à la fin d'une section \n");
        // Si on est à l'endroit auquel on veut se rendre
        if(rbt->numero_section == numero_section_objectif)
        {
            return 0;
        }
        // sinon, on se rend à la section d'après
        indice_prochaine_section = ((rbt->numero_section) + 1) % NOMBRE_SECTIONS_PRINCIPALES;
        indice_prochain_wayPoint = 0;

        // le robot à la mutex pour se rendre à la prochaine section
        if (rbt->hasMutex)
        {
            resultat_deplacement = Deplacement_elementaire(rbt, s_principale[indice_prochaine_section]->point_section[indice_prochain_wayPoint]);
            if (resultat_deplacement == 0)
            {
                actualiseSectionWayPointRobot(rbt, indice_prochaine_section, indice_prochain_wayPoint);
                //printf("On vient d'arriver à la section %d, on va lacher la mutex de la section precedente \n", indice_prochaine_section);
                pthread_mutex_unlock(&s_principale[derniere_section]->mutex);
                rbt->hasMutex = 0;
                derniere_section = indice_prochaine_section;
            }
        }
        else
        {
            if (pthread_mutex_trylock(&s_principale[indice_prochaine_section]->mutex) == 0)
            {
                //printf("On vient de prendre la mutex de la section %d \n", indice_prochaine_section);
                rbt->hasMutex = 1;
            }
        }
    }
    return 1;
}

int chercheObjet(robot* rbt, ItemPath objet)
{
    // le robot n'est as encore dans l'allee
    if(!rbt->isInAisle)
    {
        // le robot a deja pris la mutex pour se deplacer vers une allee
        if(rbt->prochaineAllee != -1)
        {
            //printf("Le robot a déjà pris l'allée %d \n", rbt->prochaineAllee);
            int entreeAllee = 1 + 2*(rbt->prochaineAllee-1);
            if(deplacementSection(rbt,entreeAllee) == 0)
            {
                printf("On vient d'arriver devant l'allée %d \n", rbt->prochaineAllee);
                rbt->isInAisle = 1;
                rbt->numero_wayPoint = 0; // wayPoint DANS L'ALLEE
            }
        }
        // si le robot n'a pas encore pris une allee
        else
        {
            printf("Le robot n'a pas encore pris d'allée \n");
            if(objet.aisleR != -1)
            {
                if(pthread_mutex_trylock(&allee_etageres[objet.aisleR - 1]->mutex) == 0)
                {
                    //si l'allee droite est dispo : on prend l'allee droite
                    printf("Le robot va se rendre dans l'allée %d \n", objet.aisleR);
                    rbt->prochaineAllee = objet.aisleR;
                    rbt->cheminAllee[0] = objet.waypointsR[0];
                    rbt->cheminAllee[1] = objet.waypointsR[1];
                    return 1;
                }
            }
            if(objet.aisleL != -1)
            {
                if(pthread_mutex_trylock(&allee_etageres[objet.aisleL - 1]->mutex) == 0)
                {
                    // si l'allée gauche est dispo : on prend l'allee gauche
                    printf("Le robot va se rendre dans l'allée %d \n", objet.aisleL);
                    rbt->prochaineAllee = objet.aisleL;
                    rbt->cheminAllee[0] = objet.waypointsL[0];
                    rbt->cheminAllee[1] = objet.waypointsL[1];
                    return 1;
                }
            }
        }
        return 1;
    }
    // si le robot a déjà atteint l'entrée de l'allée
    else
    {
        int allee_choisie = rbt->prochaineAllee;
        // wayPoint à 0 signifie qu'il est entre l'entrée de l'allée et le premier wayPoint de l'allée
        if(rbt->numero_wayPoint == 0)
        {
            // le robot vient d'arriver au premier waypoint de l'allée
            if(Deplacement_elementaire(rbt, rbt->cheminAllee[0]) == 0)
            {
                // on deblocke la mutex de l'entrée
                pthread_mutex_unlock(&allee_etageres[allee_choisie-1]->mutex);
                rbt->numero_wayPoint = 1;
            }
        }
        else if(rbt->numero_wayPoint == 1)
        {
            // le robot vient d'arriver au deuxième waypoint de l'allée
            if(Deplacement_elementaire(rbt, rbt->cheminAllee[1]) == 0)
            {
                rbt->numero_wayPoint = 2;
                return 0;
            }
        }
    }
    return 1;

}

void testAvancer()
{
    sfVector2f pos = {0,0};
    Deplacement_elementaire(rbt,pos);
}