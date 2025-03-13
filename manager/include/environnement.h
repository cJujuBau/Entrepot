// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/environnement.h
#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include <SFML/Graphics.h>
#include <sections.h> // Inclure sections.h pour accéder aux constantes

extern sfRenderWindow *window; // Déclaration de la variable externe
extern int compteur; // Déclaration de la variable externe
extern float VITESSE_ROBOT; // Déclaration de la variable externe
extern sfRectangleShape *etagere[]; // Déclaration de la variable externe
extern sfRectangleShape *bac[]; // Déclaration de la variable externe

void setupEnvironment();
void processEvents();
void render();
void clean();

#endif

