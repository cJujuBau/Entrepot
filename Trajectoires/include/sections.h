// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/sections.h
#ifndef SECTIONS_H
#define SECTIONS_H

#include <SFML/Graphics.h>
#include <pthread.h>

#define NOMBRE_ETAGERES 4
#define NOMBRE_ALLEES (NOMBRE_ETAGERES - 1)
#define NOMBRE_ROBOTS 3
#define NOMBRE_SECTIONS_PRINCIPALES (2*NOMBRE_ETAGERES + 2)
#define LARGEUR_SECTION 100
#define LONGUEUR_ETAGERE 400
#define LARGEUR_ETAGERE LARGEUR_BAC
#define LONGUEUR_BAC 250
#define LARGEUR_BAC 100
#define ECART_LONGUEUR 200
#define LONGUEUR_ENVIRONNEMENT 1650
#define LARGEUR_ENVIRONNEMENT 1000
#define LONGUEUR_ALLEE_BAC 100
#define LARGEUR_ALLEE (( LONGUEUR_ENVIRONNEMENT - 2 * ECART_LONGUEUR - LARGEUR_ETAGERE ) / (NOMBRE_ETAGERES - 1) - LARGEUR_ETAGERE)

typedef struct section_cycle_principal
{
    int nombre_points;
    sfVector2f* point_section;
    pthread_mutex_t mutex;
} section_cycle_principal;

typedef struct Allee
{
    pthread_mutex_t mutex;
    //int estDispo;
} Allee;

typedef struct
{
    pthread_mutex_t mutex;
    sfVector2f boutAllee;
    //int estDispo;
} Bac;

extern section_cycle_principal* s_principale[]; // Déclaration de la variable externe
extern Allee* allee_etageres[NOMBRE_ALLEES]; // Définition de la variable externe
extern Bac* allee_bacs[2]; // Définition de la variable externe

section_cycle_principal *creer_section(int nombre_points, int* points);
void creer_cycle_principal();
void creer_allees();
void detruire_cycle_principal();
void detruire_allee();
void creer_bacs();
void detruire_bacs();

#endif