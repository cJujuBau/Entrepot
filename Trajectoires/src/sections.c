// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/sections.c
#include <sections.h>
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.h>

section_cycle_principal* s_principale[2 * NOMBRE_ETAGERES + 4]; // Définition de la variable externe

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
    return section;
}

void creer_cycle_principal()
{
    int pos0[4];
    pos0[0] =  LONGUEUR_ENVIRONNEMENT - LARGEUR_SECTION / 2;
    pos0[1] = LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC - LARGEUR_SECTION / 2;
    pos0[2] = LONGUEUR_ENVIRONNEMENT - LARGEUR_SECTION / 2;
    pos0[3] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2;
    printf("Deuxieme wayPoint section 0 : (%d,%d) \n", pos0[2],pos0[3]);
    s_principale[0] = creer_section(2,pos0);

    int pos[2];

    for(int i=1; i <= NOMBRE_ETAGERES -1 ; i++)
    {
        printf("i = %d \n", i);
        printf("LARGEUR_ALLEE : %d | LARGEUR_ETAGERE : %d \n", LARGEUR_ALLEE, LARGEUR_ETAGERE);
        printf("LONGUEUR_ENVIRONNEMENT : %d | ECART_LONGUEUR  : %d \n", LONGUEUR_ENVIRONNEMENT, ECART_LONGUEUR);
        printf("LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - %d * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2 = %d \n", i, LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2);

        pos[0] = LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2;
        pos[1] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2;
        printf("Section numero %d (devant allée) : (%d,%d) \n", 2*i-1, pos[0], pos[1]);
        s_principale[2*i-1] = creer_section(1, pos);

        if(i != (NOMBRE_ETAGERES -1))
        {
            pos[0] = LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) - LARGEUR_ETAGERE / 2;
            printf("Section numero %d (devant étagère) : (%d,%d) \n", 2*i, pos[0], pos[1]);
            s_principale[2*i] = creer_section(1, pos);
        }
    }

    pos[0] = LARGEUR_SECTION / 2;
    pos[1] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2;
    s_principale[2*(NOMBRE_ETAGERES - 1)] = creer_section(1,pos);
    pos[1] = LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC - LARGEUR_SECTION / 2;
    s_principale[2*NOMBRE_ETAGERES - 1] = creer_section(1,pos);

    pos[0] = ECART_LONGUEUR + LARGEUR_BAC / 2;
    s_principale[2*NOMBRE_ETAGERES] = creer_section(1,pos);
    pos[0] = ECART_LONGUEUR + LARGEUR_BAC + LARGEUR_ALLEE / 2;
    s_principale[2*NOMBRE_ETAGERES+1] = creer_section(1,pos);
    pos[0] = ECART_LONGUEUR +  3 * LARGEUR_BAC / 2 + LARGEUR_ALLEE;
    s_principale[2*NOMBRE_ETAGERES+2] = creer_section(1,pos);
    pos[0] = ECART_LONGUEUR +  2 * LARGEUR_BAC + 3 * LARGEUR_ALLEE / 2;
    s_principale[2*NOMBRE_ETAGERES+3] = creer_section(1,pos);
}