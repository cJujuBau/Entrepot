// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/environnement.c
#include <environnement.h>
#include <robot.h>
#include <stdio.h>
#include <SFML/Graphics.h> 

sfRenderWindow *window = NULL; // Définition de la variable externe
int compteur = 0; // Définition de la variable externe
float VITESSE_ROBOT = 0.2; // Définition de la variable externe
sfRectangleShape *etagere[NOMBRE_ETAGERES]; // Définition de la variable externe
sfRectangleShape *bac[2]; // Définition de la variable externe

extern robot* rbt;
extern robot* rbt2;
extern robot* rbt3;

sfRectangleShape* P1;
sfRectangleShape* P2;

void setupEnvironment() {
    sfVideoMode mode = {LONGUEUR_ENVIRONNEMENT, LARGEUR_ENVIRONNEMENT, 32}; // Définition de la variable mode
    window = sfRenderWindow_create(mode, "CSFML Example", sfResize | sfClose, NULL);
    if (!window) {
        printf("Erreur: Impossible de créer la fenêtre.\n");
        return;
    }

    int longueurTotaleEtageres = LONGUEUR_ENVIRONNEMENT - 2 * ECART_LONGUEUR - LARGEUR_ETAGERE;
    float espacement = (float)longueurTotaleEtageres / (NOMBRE_ETAGERES - 1);
    //printf("Espacement : %f \n", espacement);

    sfVector2f point1 = {441,350};
    sfVector2f point2 = {511,350};
    sfVector2f sizePetitCarre = {10,10};

    P1  = sfRectangleShape_create();
    P2  = sfRectangleShape_create();

    sfRectangleShape_setSize(P1, sizePetitCarre);
    sfRectangleShape_setFillColor(P1, sfYellow);
    sfRectangleShape_setPosition(P1, point1);

    sfRectangleShape_setSize(P2, sizePetitCarre);
    sfRectangleShape_setFillColor(P2, sfYellow);
    sfRectangleShape_setPosition(P2, point2);

    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        etagere[i] = sfRectangleShape_create();
        if (!etagere[i]) {
            printf("Erreur: Impossible de créer une étagère.\n");
            return;
        }

        sfVector2f size = {LARGEUR_ETAGERE, LONGUEUR_ETAGERE};    
        sfVector2f pos = {ECART_LONGUEUR + i * espacement, 0}; 
        //printf("Etagere numero %d : (%d,%d) \n", i, (int)pos.x, (int)pos.y);

        sfRectangleShape_setSize(etagere[i], size);
        sfRectangleShape_setFillColor(etagere[i], sfGreen);
        sfRectangleShape_setPosition(etagere[i], pos);
    }

    for (int i = 0; i < 2; i++) {
        bac[i] = sfRectangleShape_create();
        if (!bac[i]) {
            printf("Erreur: Impossible de créer un bac.\n");
            return;
        }

        sfVector2f size = {LARGEUR_BAC, LONGUEUR_BAC};
        sfVector2f pos = {i * (LARGEUR_ALLEE+LARGEUR_ETAGERE)+ECART_LONGUEUR, (LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC)}; 

        sfRectangleShape_setSize(bac[i], size);
        sfRectangleShape_setFillColor(bac[i], sfGreen);
        sfRectangleShape_setPosition(bac[i], pos);
    }

    creer_robot(&rbt,0,1);
    creer_robot(&rbt2,3,0);
    creer_robot(&rbt3,5,0);
}

void processEvents()
{
    sfEvent event;
    while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtClosed) {
            sfRenderWindow_close(window);
        }
    }
}

void render()
{
    sfRenderWindow_clear(window, sfBlack);

    sfRenderWindow_drawRectangleShape(window, P1 , NULL);
    sfRenderWindow_drawRectangleShape(window, P2 , NULL);
    
    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        sfRenderWindow_drawRectangleShape(window, etagere[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        sfRenderWindow_drawRectangleShape(window, bac[i], NULL);
    }

    sfRenderWindow_drawCircleShape(window, rbt->cercle, NULL);
    sfRenderWindow_drawCircleShape(window, rbt2->cercle, NULL);
    sfRenderWindow_drawCircleShape(window, rbt3->cercle, NULL);

    sfRenderWindow_display(window);
}

void clean()
{
    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        sfRectangleShape_destroy(etagere[i]);
    }
    for (int i = 0; i < 2; i++) {
        sfRectangleShape_destroy(bac[i]);
    }
    sfCircleShape_destroy(rbt->cercle);
    sfCircleShape_destroy(rbt2->cercle);
    sfCircleShape_destroy(rbt3->cercle);
    sfRenderWindow_destroy(window);
}