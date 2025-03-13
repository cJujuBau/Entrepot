// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include <SFML/Graphics.h>

enum {etapeDeplacementAvantCollecte, etapeCollecte, etapeRetourCyclePrincipal, etapeCollecte2, etapeCollecteDansMemeAllee, etapeRetourCyclePrincipal2, etapeDeplacementAvantDepose, etapeDepose};

typedef struct robot
{
    int id_robot;
    int etape;
    sfVector2f* pos;
    sfCircleShape* cercle; 
    int numero_section;
    int numero_wayPoint;
    int hasMutex; // 1 si le robot a pris la mutex de la section où il veut se rendre, 0 sinon
    int prochaineAllee;
    int isInAisle;
    sfVector2f *cheminAllee;
    int nombreObjetsAChercher;
} robot;

typedef struct {
    int id;
    int bac;
    int rack, row;
    int quantity;
    int aisleL, aisleR;
    sfVector2f *waypointsL, *waypointsR;
} ItemPath;

extern robot* rbt; // Déclaration de la variable externe
extern robot* rbt2; // Déclaration de la variable externe
extern robot* rbt3; // Déclaration de la variable externe

void creer_robot(robot** r, int n_section, int n_wayPoint, int id);
void actualisePositionRobot(robot* rbt, sfVector2f nouvellePosition);
void actualiseSectionWayPointRobot(robot* rbt, int n_section, int n_wayPoint);
int Deplacement_elementaire(robot* rbt, sfVector2f posfinale);
int deplacementSection(robot* rbt, int numero_section_objectif);
void testAvancer();
int chercheObjet(robot* rbt, ItemPath* objet);
int retourCyclePrincipal(robot* rbt);
int deposeBac(robot* rbt, int bac);
int estDansLaMemeAllee(robot* rbt, ItemPath* objet);
int chercheAutreObjetDansAllee(robot* rbt);
void actualiseChemin(robot* rbt, sfVector2f* chemin);

#endif