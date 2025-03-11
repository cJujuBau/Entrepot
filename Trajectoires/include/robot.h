// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include <SFML/Graphics.h>

typedef struct robot
{
    sfVector2f* pos;
    sfCircleShape* cercle; 
    int numero_section;
    int numero_wayPoint;
} robot;

extern robot* rbt; // Déclaration de la variable externe

void creer_robot(robot** r, int n_section, int n_wayPoint);
void actualisePositionRobot(robot* rbt, sfVector2f nouvellePosition);
void actualiseSectionWayPointRobot(robot* rbt, int n_section, int n_wayPoint);
int Deplacement_elementaire(robot* rbt, sfVector2f posfinale);
int deplacementSection(robot* rbt, int numero_section_objectif);
void testAvancer();

#endif