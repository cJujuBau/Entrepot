#include <SFML/Graphics.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define NOMBRE_ETAGERES 4
#define NOMBRE_ALLEES (NOMBRE_ETAGERES - 1)
#define NOMBRE_ROBOTS 3
#define NOMBRE_SECTIONS_PRINCIPALES (2*NOMBRE_ETAGERES + 4) // attention il faut que cette variable soit dynamique
#define LARGEUR_SECTION 200 // 200
#define VITESSE_ROBOT 0.15
#define LONGUEUR_ETAGERE 400 // 400
#define LARGEUR_ETAGERE 100 // 100 
#define LONGUEUR_BAC 250
#define LARGEUR_BAC 100 // 100
#define ECART_LONGUEUR 200 //200
#define LONGUEUR_ENVIRONNEMENT 1650 // 1650
#define LARGEUR_ENVIRONNEMENT 1000
#define LARGEUR_ALLEE (( LONGUEUR_ENVIRONNEMENT - 2 * ECART_LONGUEUR - LARGEUR_ETAGERE ) / (NOMBRE_ETAGERES - 1) - LARGEUR_ETAGERE)

sfVideoMode mode = {LONGUEUR_ENVIRONNEMENT, LARGEUR_ENVIRONNEMENT, 32};
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

// met à jour le numero de la section et du wayPoint du robot
void actualiseSectionWayPointRobot(robot* rbt, int n_section, int n_wayPoint)
{
    rbt->numero_section = n_section;
    rbt->numero_wayPoint = n_wayPoint;
}

// fonction qui rée le robot utilisé pour les tests à la section n_section et au wayPoint n_wayPoint
void creer_robot(robot** r, int n_section, int n_wayPoint) {
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
    actualiseSectionWayPointRobot(rbt,n_section,n_wayPoint);
    sfVector2f pos = s_principale[n_section]->point_section[n_wayPoint];
    actualisePositionRobot(rbt,pos);
    sfCircleShape_setRadius(rbt->cercle, 15); // rayon fixe pour le moment
    sfCircleShape_setFillColor(rbt->cercle,sfRed);
}

// creer l'environneùent de travail
void setupEnvironment() {

    // creation de la fenetre
    window = sfRenderWindow_create(mode, "CSFML Example", sfResize | sfClose, NULL);
    if (!window) {
        printf("Erreur: Impossible de créer la fenêtre.\n");
        return;
    }

    int longueurTotaleEtageres = LONGUEUR_ENVIRONNEMENT - 2 * ECART_LONGUEUR - LARGEUR_ETAGERE;
    float espacement = (float)longueurTotaleEtageres / (NOMBRE_ETAGERES - 1);
    printf("Espacement : %f \n", espacement);

    for (int i = 0; i < NOMBRE_ETAGERES; i++) {
        etagere[i] = sfRectangleShape_create();
        if (!etagere[i]) {
            printf("Erreur: Impossible de créer une étagère.\n");
            return;
        }

        sfVector2f size = {LARGEUR_ETAGERE, LONGUEUR_ETAGERE};    
        sfVector2f pos = {ECART_LONGUEUR + i * espacement, 0}; 
        printf("Etagere numero %d : (%d,%d) \n", i, (int)pos.x, (int)pos.y);

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

        sfVector2f size = {LARGEUR_BAC, LONGUEUR_BAC};
        //sfVector2f pos = {i * (longueurTotaleEtageres  / NOMBRE_ETAGERES)+ECART_LONGUEUR, (LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC)}; 
        sfVector2f pos = {i * (LARGEUR_ALLEE+LARGEUR_ETAGERE)+ECART_LONGUEUR, (LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC)}; 

        sfRectangleShape_setSize(bac[i], size);
        sfRectangleShape_setFillColor(bac[i], sfGreen);
        sfRectangleShape_setPosition(bac[i], pos);
    }

    // dessin d'un robot pour le moment (a adapter quand on augmente le nombre de robots)
    creer_robot(&rbt,3,0); // section 0, wayPoint 0
    
}

// creation des sections du cycle principal
void creer_cycle_principal()
{
    // sfVector2f pos = {i * (1400 / NOMBRE_ETAGERES)+300, 30}; // Espacement correct

    int pos0[4];
    pos0[0] =  LONGUEUR_ENVIRONNEMENT - LARGEUR_SECTION / 2; // 1900
    pos0[1] = LARGEUR_ENVIRONNEMENT - LONGUEUR_BAC - LARGEUR_SECTION / 2; // 750
    pos0[2] = LONGUEUR_ENVIRONNEMENT - LARGEUR_SECTION / 2; // 1750
    pos0[3] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2; // 450
    printf("Deuxieme wayPoint section 0 : (%d,%d) \n", pos0[2],pos0[3]);
    s_principale[0] = creer_section(2,pos0); // je créer la section 0 du cycle principal

    int pos[2];

    for(int i=1; i <= NOMBRE_ETAGERES -1 ; i++)
    {
        printf("i = %d \n", i);
        printf("LARGEUR_ALLEE : %d | LARGEUR_ETAGERE : %d \n", LARGEUR_ALLEE, LARGEUR_ETAGERE);
        printf("LONGUEUR_ENVIRONNEMENT : %d | ECART_LONGUEUR  : %d \n", LONGUEUR_ENVIRONNEMENT, ECART_LONGUEUR);
        printf("LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - %d * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2 = %d \n", i, LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2);

        // Position devant l'étagère
        pos[0] = LONGUEUR_ENVIRONNEMENT - ECART_LONGUEUR - i * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2;
        pos[1] = LONGUEUR_ETAGERE + LARGEUR_SECTION / 2;
        printf("Section numero %d (devant allée) : (%d,%d) \n", 2*i-1, pos[0], pos[1]);
        s_principale[2*i-1] = creer_section(1, pos);

        // Position devant l'allée
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

// fermer la fenetre quand on clique sur la croix
void processEvents()
{
    sfEvent event;
    while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtClosed) {
            sfRenderWindow_close(window);
        }
    }
}

// mettre a jour la fenetre
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

// fonction de test unitaire pour tester si le robot arrive a avancer jusqu'à un point précis
void testAvancer()
{
    //sfVector2f pos = {((rbt->pos)->x)+1,((rbt->pos)->y)+1};
    sfVector2f pos = {0,0};
    //sfVector2f pos = *(s_principale[0]->point_section);
    Deplacement_elementaire(rbt,pos);
    //printf("Position du robot : (%f,%f) \n", (rbt->pos)->x, (rbt->pos)->y);
}

// fonction qui amène le robot au dernier wayPoint de la section passée en argument
int deplacementSection(robot* rbt, int numero_section_objectif)
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

// effacer la fenetre
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
        if(deplacementSection(rbt,2) == 0)
        {
            printf("On est arrivé ! \n");
            clean();
            return 0;
        }
        sleep(0.5);
        compteur++; // compteur utilisé pour logger pas trop souvent
    }
    clean();
    return 0;
}
