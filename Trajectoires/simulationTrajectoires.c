#include <SFML/Graphics.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NOMBRE_ETAGERES 4
#define NOMBRE_ALLEES (NOMBRE_ETAGERES - 1)
#define NOMBRE_ROBOTS 3
#define NOMBRE_SECTIONS_PRINCIPALES 2*NOMBRE_ALLEES + 7 + NOMBRE_ROBOTS
#define LARGEUR_ALLEE 50

sfVideoMode mode = {2000, 1000, 32};
sfRenderWindow *window;
sfRectangleShape *shape[NOMBRE_ETAGERES];
sfRectangleShape *bac[2]; // 2 bacs : parametre non scalable

typedef struct section_cycle_principal
{
    int nombre_points; // nombre de points physiques affiliés à la section
    sfVector2f* point_section; // liste des points physiques affiliés à la section
}section_cycle_principal;

typedef struct robot
{
    sfVector2f* pos;
}robot;

section_cycle_principal* s_principale[NOMBRE_SECTIONS_PRINCIPALES]; // liste des sections principales
robot* rbt;

section_cycle_principal *creer_section(int nombre_points, int* points) {
    section_cycle_principal *section = malloc(sizeof(section_cycle_principal));
    if (!section) return NULL; // Vérifier si l'allocation a réussi

    section->nombre_points = nombre_points;
    section->point_section = malloc(nombre_points * sizeof(sfVector2f));

    if (!section->point_section) {
        free(section); // Libérer la mémoire de la structure si l'allocation échoue
        return NULL;
    }

    if (section) {
        for(int i=0; i<nombre_points; i++)
        {
            section->point_section[i] = (sfVector2f){points[2*i],points[2*i]+1};
        }
    }

    return section;
}

void setupEnvironment() {
    window = sfRenderWindow_create(mode, "CSFML Example", sfResize | sfClose, NULL);
    if (!window) {
        printf("Erreur: Impossible de créer la fenêtre.\n");
        return;
    }

    // creation des etageres
    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        shape[i] = sfRectangleShape_create();
        if (!shape[i]) {
            printf("Erreur: Impossible de créer un rectangle.\n");
            return;
        }

        sfVector2f size = {100, 400};
        sfVector2f pos = {i * (1400 / NOMBRE_ETAGERES)+300, 30}; // Espacement correct

        sfRectangleShape_setSize(shape[i], size);
        sfRectangleShape_setFillColor(shape[i], sfGreen);
        sfRectangleShape_setPosition(shape[i], pos);
    }

    //creation des bacs
    for (int i = 0; i < 2; i++) {
        bac[i] = sfRectangleShape_create();
        if (!bac[i]) {
            printf("Erreur: Impossible de créer un bac.\n");
            return;
        }

        sfVector2f size = {100, 250};
        sfVector2f pos = {i * (1400 / NOMBRE_ETAGERES)+300, 750}; // Espacement correct

        sfRectangleShape_setSize(bac[i], size);
        sfRectangleShape_setFillColor(bac[i], sfGreen);
        sfRectangleShape_setPosition(bac[i], pos);
    }

}

void creer_cycle_principal()
{
    int pos[2];
    pos[0] = 300;
    pos[1] = 30;
    s_principale[0] = creer_section(1,pos); // je créer la section 0 du cycle principal
    int pos2[4];
    pos2[0] = 400;
    pos2[1] = 30;
    pos2[2] = 400;
    pos2[3] = 80;
    s_principale[1] = creer_section(2,pos2);
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
    
    // Dessiner chaque etagere
    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        sfRenderWindow_drawRectangleShape(window, shape[i], NULL);
    }

    // Dessiner chaque bac
    for (int i = 0; i < 2; i++) {
        sfRenderWindow_drawRectangleShape(window, bac[i], NULL);
    }
    sfRenderWindow_display(window);
}

void Deplacement_elementaire(sfVector2f posfinale, robot* rbt)
{
    int pente = (posfinale.y - (rbt->pos)->y) / (posfinale.x - (rbt->pos)->x);
    int distance = sqrt((posfinale.y - (rbt->pos)->y)*(posfinale.y - (rbt->pos)->y) + (posfinale.x - (rbt->pos)->x)*(posfinale.x - (rbt->pos)->x));
    for (int i=0; i<10; i++)
    {
        (rbt->pos)->x += pente * 
    }
}

void clean()
{
    // Libération de la mémoire

    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        sfRectangleShape_destroy(shape[i]);
    }
    for (int i = 0; i < 2; i++) {
        sfRectangleShape_destroy(bac[i]);
    }
    sfRenderWindow_destroy(window);
}

int main() 
{
    setupEnvironment();

    rbt->pos = s_principale[0]->point_section;

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();
    }
    clean();
    return 0;
}
