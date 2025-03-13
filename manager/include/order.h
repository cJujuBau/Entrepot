/* ------------------------------------------------------------------------ */
/*                       Entrepot - Robots - main.h                         */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */


#ifndef __ORDER_H
#define __ORDER_H 1

/* ------------------------------------------------------------------------ */
/*                        S T A N D A R D   H E A D E R S                   */
/* ------------------------------------------------------------------------ */

#include <utils.h>
#include <jansson.h>
#include <SFML/Graphics.h>

/* ------------------------------------------------------------------------ */
/*                    S Y M B O L I C   C O N S T A N T S                   */
/* ------------------------------------------------------------------------ */

#ifndef SECTIONS_H
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
#endif

#define NB_ITEMS_PER_ROBOT 2
#define NB_ITEMS_PAR_ALLEE 4
#define LONGUEUR_ETAGERE_PAR_ITEM (LONGUEUR_ETAGERE/NB_ITEMS_PAR_ALLEE)

#define MAX_AISLE NOMBRE_ALLEES
#define MIN_AISLE 1

/* ------------------------------------------------------------------------ */
/*                        T Y P E   D E F I N I T I O N S                   */
/* ------------------------------------------------------------------------ */

typedef struct {
    int id;
    int bac;
    int rack, row;
    int quantity;
    int aisleL, aisleR;
    sfVector2f *waypointsL, *waypointsR;
} ItemPath;

typedef struct OrdersElementary
{
    int nbOrdersElementary;
    int sizeOrdersElementary;
    ItemPath **items;
} *OrdersElementary;

/* ------------------------------------------------------------------------ */
/*                        G L O B A L   V A R I A B L E S                   */
/* ------------------------------------------------------------------------ */

extern OrdersElementary ordersElementaryGlobal;

/* ------------------------------------------------------------------------ */
/*                    F U N C T I O N   P R O T O T Y P E S                 */
/* ------------------------------------------------------------------------ */

void newOrder();
void *threadWaitOrder(void *arg);
void processOrder(char *orderStr);
void constructAisle(ItemPath *order, int nbItems);
void constructWaypoint(ItemPath *order, int nbItems);

int extractIdQty(char *orderStr, int **idItemsOrder, int **qtyItemsOrder);
ItemPath *load_inventory(const char *path, int nbItems, int *idItemsOrder, int *qtyItemsOrder);

int divideOrder(ItemPath *order, int nbItems, int ***idItemsOrderElementary);
int attributeBac(ItemPath *order, int nbItems);

void addOrdersToOrdersGlobal(int **idItemsOrderElementary, int nbOrdersElementary, ItemPath *order);
void removeOrderFromOrdersGlobal(int index);
void printOrdersElementary();
void freeOrdersElementary();

#endif