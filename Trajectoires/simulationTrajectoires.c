#include <SFML/Graphics.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define NOMBRE_ETAGERES 4
#define NOMBRE_ALLEES (NOMBRE_ETAGERES - 1)
#define NOMBRE_ROBOTS 3
//#define NOMBRE_SECTIONS_PRINCIPALES 2*NOMBRE_ALLEES + 7 + NOMBRE_ROBOTS // attention il faut que cette variable soit dynamique
#define NOMBRE_SECTIONS_PRINCIPALES 4
#define LARGEUR_ALLEE 50
#define VITESSE_ROBOT 0.2

sfVideoMode mode = {2000, 1000, 32};
sfRenderWindow *window;
sfRectangleShape *etagere[NOMBRE_ETAGERES];
sfRectangleShape *bac[2]; // 2 bacs : parametre non scalable

int compteur;

typedef struct section_cycle_principal
{
    int nombre_points; // nombre de points physiques affiliés à la section
    sfVector2f* point_section; // liste des points physiques affiliés à la section
}section_cycle_principal;

typedef struct robot
{
    sfVector2f* pos;
    sfCircleShape* cercle; 
    int numero_section; // numero de la section courante dans laquelle le robot se situe ( cycle princpal ou non )
    int numero_wayPoint; // numero du point intermédiaire dans la section ( demarrage à 0 ). Si le robot fait a ---> b, entre a et b il possède le wayPoint a en mémoire
}robot;

section_cycle_principal* s_principale[NOMBRE_SECTIONS_PRINCIPALES]; // liste des sections principales
robot* rbt = NULL;

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
            section->point_section[i] = (sfVector2f){points[2*i],points[2*i+1]};
        }
    }
    return section;
}


// fonction qui actualise la position du robot dans la structure et pour l'affichage
void actualisePositionRobot(robot* rbt, sfVector2f nouvellePosition)
{
    (rbt->pos)->x = nouvellePosition.x;
    (rbt->pos)->y = nouvellePosition.y;
    sfCircleShape_setPosition(rbt->cercle,nouvellePosition);
}

void creer_robot(robot** r) {
    *r = malloc(sizeof(robot)); // Alloue la mémoire pour le robot
    if (*r == NULL) {
        perror("Erreur malloc robot");
        exit(EXIT_FAILURE);
    }

    (*r)->pos = malloc(sizeof(sfVector2f)); // Alloue la mémoire pour pos
    if ((*r)->pos == NULL) {
        perror("Erreur malloc pos");
        free(*r);
        exit(EXIT_FAILURE);
    }
    rbt->cercle = sfCircleShape_create();
    rbt->numero_section = 2; // section initiale du robot, fixe pour le moment
    rbt->numero_wayPoint = 1; // wayPoint initial
    sfVector2f pos = s_principale[2]->point_section[1];
    actualisePositionRobot(rbt,pos);
    sfCircleShape_setRadius(rbt->cercle, 25); // rayon fixe pour le moment
    sfCircleShape_setFillColor(rbt->cercle,sfRed);
}

void actualiseSectionWayPointRobot(robot* rbt, int n_section, int n_wayPoint)
{
    rbt->numero_section = n_section;
    rbt->numero_wayPoint = n_wayPoint;
}

void setupEnvironment() {
    window = sfRenderWindow_create(mode, "CSFML Example", sfResize | sfClose, NULL);
    if (!window) {
        printf("Erreur: Impossible de créer la fenêtre.\n");
        return;
    }

    // creation des etageres
    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        etagere[i] = sfRectangleShape_create();
        if (!etagere[i]) {
            printf("Erreur: Impossible de créer une etagere.\n");
            return;
        }

        sfVector2f size = {100, 400};
        sfVector2f pos = {i * (1400 / NOMBRE_ETAGERES)+300, 30}; // Espacement correct

        sfRectangleShape_setSize(etagere[i], size);
        sfRectangleShape_setFillColor(etagere[i], sfGreen);
        sfRectangleShape_setPosition(etagere[i], pos);
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

    // dessin d'un robot pour le moment (a adapter quand on augmente le nombre de robots)
    creer_robot(&rbt);
    //sfVector2f pos = {300,450};
    
}

void creer_cycle_principal()
{
    int pos[2];
    pos[0] = 400;
    pos[1] = 450;
    s_principale[0] = creer_section(1,pos); // je créer la section 0 du cycle principal
    int pos2[2];
    pos2[0] = 500;
    pos2[1] = 450;
    s_principale[1] = creer_section(1,pos2);
    //printf("Section 1 à la création : (%f,%f) \n", s_principale[1]->point_section->x, s_principale[1]->point_section->y);
    int pos3[4];
    pos3[0] = 600;
    pos3[1] = 450;
    pos3[2] = 600;
    pos3[3] = 600;
    s_principale[2] = creer_section(2,pos3);
    int pos4[6];
    pos4[0] = 600;
    pos4[1] = 500;
    pos4[2] = 700;
    pos4[3] = 500;
    pos4[4] = 600;
    pos4[5] = 600;
    s_principale[3] = creer_section(3,pos4);
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
        sfRenderWindow_drawRectangleShape(window, etagere[i], NULL);
    }

    // Dessiner chaque bac
    for (int i = 0; i < 2; i++) {
        sfRenderWindow_drawRectangleShape(window, bac[i], NULL);
    }

    // dessiner un robot pour le moment
    sfRenderWindow_drawCircleShape(window, rbt->cercle, NULL);

    sfRenderWindow_display(window);
}

// fonction qui renvoie la distance entre 2 points posA et posB
float distance(sfVector2f posA, sfVector2f posB)
{
    return sqrt((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y));
}

// fonction qui fait avancer le robot vers son point final
int Deplacement_elementaire(robot* rbt, sfVector2f posfinale)
{
    sfVector2f posRobot = *(rbt->pos);
    float d = distance(posRobot,posfinale);
    if(d > 5) // critère de tolérance pour considérer que le robot est bien à la position finale
    {
        //printf("Valeur x de posfinale : %f | valeur x de posRobot : %f | ratio : %f \n", posfinale.x, posRobot.x, (posfinale.x - posRobot.x)/d);
        sfVector2f nouvellePosition = {(rbt->pos)->x + VITESSE_ROBOT * ((posfinale.x - posRobot.x)/d), (rbt->pos)->y + VITESSE_ROBOT * ((posfinale.y - posRobot.y)/d)}; 
        if(compteur  % 100 == 0)
        printf("Nouvelle position du robot : (%f,%f) \n", nouvellePosition.x, nouvellePosition.y);
        actualisePositionRobot(rbt,nouvellePosition);
        return 1;
    }
    return 0; // on est arrivé à destination
}

void testAvancer()
{
    //sfVector2f pos = {((rbt->pos)->x)+1,((rbt->pos)->y)+1};
    sfVector2f pos = {0,0};
    //sfVector2f pos = *(s_principale[0]->point_section);
    Deplacement_elementaire(rbt,pos);
    //printf("Position du robot : (%f,%f) \n", (rbt->pos)->x, (rbt->pos)->y);
}

// fonction qui amène le robot au dernier wayPoint de la section passée en argument
int testDeplacement(int numero_section_objectif)
{
    int resultat_deplacement;
    int indice_prochaine_section;
    int indice_prochain_wayPoint;
    
    if(rbt->numero_wayPoint < (s_principale[(rbt->numero_section)]->nombre_points)-1) // si on n'est pas au dernier wayPoint de sa section ( section à + d'1 wayPoint )
    {
        indice_prochaine_section = rbt->numero_section ; // on reste dans la même section
        indice_prochain_wayPoint = rbt->numero_wayPoint + 1; // on veut progresser dans la section courante
    }
    else // on est au dernier wayPoint de sa section
    {
        if(rbt->numero_section == numero_section_objectif) // on est a la section objectif
        {
            return 0; // on est arrivé à destination
        }
        else // on n'est pas à la section objectif : on doit se déplacer à la prochaine section
        {
            indice_prochaine_section = ((rbt->numero_section) + 1) % NOMBRE_SECTIONS_PRINCIPALES; // on doit se rendre à la section qui suit 
            indice_prochain_wayPoint = 0; // on doit se rendre au premier wayPoint de la section qui suit
        }
    }

    resultat_deplacement = Deplacement_elementaire(rbt,s_principale[indice_prochaine_section]->point_section[indice_prochain_wayPoint]);
    if(resultat_deplacement == 0) // si on est arrivé au point intermediaire
    {
        actualiseSectionWayPointRobot(rbt,indice_prochaine_section,indice_prochain_wayPoint);
    }
    return 1;

}

void clean()
{
    // Libération de la mémoire

    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        sfRectangleShape_destroy(etagere[i]);
    }
    for (int i = 0; i < 2; i++) {
        sfRectangleShape_destroy(bac[i]);
    }
    sfCircleShape_destroy(rbt->cercle);
    sfRenderWindow_destroy(window);
}

int main() 
{

    creer_cycle_principal();    

    setupEnvironment();

    if (!window) return 1; // Vérification si la fenêtre a bien été créée

    while (sfRenderWindow_isOpen(window)) 
    {
        processEvents();
        render();
        //testAvancer();
        if(testDeplacement(0) == 0)
        {
            //printf("On est arrivé ! \n");
        }
        sleep(0.5);
        compteur++;
    }
    clean();
    return 0;
}
