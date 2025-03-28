// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/sections.c
#include <sections.h>
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.h>

section_cycle_principal* s_principale[2 * NOMBRE_ETAGERES + 4]; // Définition de la variable externe
Allee* allee_etageres[NOMBRE_ALLEES]; // Définition de la variable externe
Bac* allee_bacs[2]; // Définition de la variable externe

section_cycle_principal *creer_section(int nombre_points, int* points) {
    section_cycle_principal *section = malloc(sizeof(section_cycle_principal));
    if (!section) return NULL;

    section->nombre_points = nombre_points;
    section->point_section = malloc(nombre_points * sizeof(sfVector2f));

    if (!section->point_section) {
        free(section);
        return NULL;
    }

    for(int i=0; i<nombre_points; i++)
    {
        section->point_section[i] = (sfVector2f){points[2*i],points[2*i+1]};
    }

    // Initialiser le mutex
    if (pthread_mutex_init(&section->mutex, NULL) != 0) {
        printf("Erreur: Impossible d'initialiser le mutex.\n");
        free(section->point_section);
        free(section);
        return NULL;
    }
    return section;
}

void creer_cycle_principal()
{
    int pos2[4];
    pos2[0] =  LONGUEUR_ENVIRONNEMENT - LARGEUR_SECTION / 2;
    pos2[1] = LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC - LONGUEUR_ALLEE_BAC - LARGEUR_SECTION / 2;
    pos2[2] = LONGUEUR_ENVIRONNEMENT - LARGEUR_SECTION / 2;
    pos2[3] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2;
    //printf("Deuxieme wayPoint section 0 : (%d,%d) \n", pos2[2],pos2[3]);
    s_principale[0] = creer_section(2,pos2);

    int pos[2];

    for(int i=1; i <= NOMBRE_ETAGERES -1 ; i++)
    {
        // printf("i = %d \n", i);
        // printf("LARGEUR_ALLEE : %d | LARGEUR_ETAGERE : %d \n", LARGEUR_ALLEE, LARGEUR_ETAGERE);
        // printf("LONGUEUR_ENVIRONNEMENT : %d | ECART_LONGUEUR  : %d \n", LONGUEUR_ENVIRONNEMENT, ECART_LONGUEUR);
        // printf("LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - %d * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2 = %d \n", i, LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2);

        pos[0] = LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2;
        pos[1] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2;
        //printf("Section numero %d (devant allée) : (%d,%d) \n", 2*i-1, pos[0], pos[1]);
        s_principale[2*i-1] = creer_section(1, pos);

        if(i != (NOMBRE_ETAGERES -1))
        {
            pos[0] = LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) - LARGEUR_ETAGERE / 2;
            printf("Section numero %d (devant étagère) : (%d,%d) \n", 2*i, pos[0], pos[1]);
            s_principale[2*i] = creer_section(1, pos);
        }
    }

    pos2[0] = LARGEUR_SECTION / 2;
    pos2[1] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2;
    pos2[2] = LARGEUR_SECTION / 2;
    pos2[3] = LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC - LONGUEUR_ALLEE_BAC - LARGEUR_SECTION / 2;
    s_principale[2*(NOMBRE_ETAGERES - 1)] = creer_section(2,pos2);
    pos[0] = ECART_LONGUEUR + LARGEUR_BAC / 2;
    pos[1] = LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC - LONGUEUR_ALLEE_BAC -  LARGEUR_SECTION / 2;
    s_principale[2*NOMBRE_ETAGERES - 1] = creer_section(1,pos);

    pos[0] = ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE / 2;
    s_principale[2*NOMBRE_ETAGERES] = creer_section(1,pos);
    pos[0] = ECART_LONGUEUR + LARGEUR_ALLEE + 3*LARGEUR_BAC / 2;
    s_principale[2*NOMBRE_ETAGERES+1] = creer_section(1,pos);
}

void detruire_cycle_principal()
{
    for (int i = 0; i < NOMBRE_SECTIONS_PRINCIPALES; i++) {
        if (s_principale[i] != NULL) {
            pthread_mutex_destroy(&s_principale[i]->mutex); // Détruire le mutex
            free(s_principale[i]->point_section);
            free(s_principale[i]);
        }
    }
}

void creer_allees()
{
    for (int i = 0; i < NOMBRE_ALLEES; i++) {
        allee_etageres[i] = malloc(sizeof(Allee));
        if (allee_etageres[i] == NULL) {
            perror("Erreur malloc allee");
            free(allee_etageres[i]);
            exit(EXIT_FAILURE);
        }
        // Initialiser le mutex
        if (pthread_mutex_init(&allee_etageres[i]->mutex, NULL) != 0) {
            printf("Erreur: Impossible d'initialiser le mutex.\n");
            free(allee_etageres[i]);
            exit(EXIT_FAILURE);
        }
    }
}

void detruire_allee()
{
    for (int i = 0; i < NOMBRE_ALLEES; i++) {
        if (allee_etageres[i] != NULL) {
            pthread_mutex_destroy(&allee_etageres[i]->mutex); // Détruire le mutex
            free(allee_etageres[i]);
        }
    }
}

void creer_bacs()
{
    for (int i = 0; i < 2; i++) {
        allee_bacs[i] = malloc(sizeof(Bac));
        if (allee_bacs[i] == NULL) {
            perror("Erreur malloc bac");
            free(allee_bacs[i]);
            exit(EXIT_FAILURE);
        }
        // Initialiser le mutex
        if (pthread_mutex_init(&allee_bacs[i]->mutex, NULL) != 0) {
            printf("Erreur: Impossible d'initialiser le mutex.\n");
            free(allee_bacs[i]);
            exit(EXIT_FAILURE);
        }
    }
    allee_bacs[0]->boutAllee = (sfVector2f) {ECART_LONGUEUR + LARGEUR_BAC / 2, LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC};
    allee_bacs[1]->boutAllee = (sfVector2f) {ECART_LONGUEUR + LARGEUR_ALLEE + 3 * LARGEUR_BAC / 2, LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC};
}

void detruire_bacs()
{
    for (int i = 0; i < 2; i++) {
        if (allee_bacs[i] != NULL) {
            pthread_mutex_destroy(&allee_bacs[i]->mutex); // Détruire le mutex
            free(allee_bacs[i]);
        }
    }
}