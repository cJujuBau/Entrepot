#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <utils.h>
#include <order.h>
#include <path.h>

#include <jansson.h>
#include <SFML/Graphics.h>



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

#define MAX_AISLE NOMBRE_ETAGERES
#define MIN_AISLE 1

// Thread to wait for a new order
void *threadWaitOrder(void *arg){
    while(1){
        pause();
    }
}

int openSharedMemory(void **pShm){
    // Open or create the shared memory segment
    int shmFd = shm_open(SHM_ORDER, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    CHECK(shmFd, "Failed to open shared memory");

    // Associate the memory segment with the process's address space
    CHECK(ftruncate(shmFd, SHM_SIZE), "Failed to set the size of shared memory");

    *pShm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    CHECK_NULL(*pShm, "Failed to map shared memory");

    // Close the file descriptor after mapping
    CHECK(close(shmFd), "Failed to close shared memory file descriptor");

    return shmFd;
} 

void newOrder(){
    // read shared memory
    void *pShm = NULL;
    int shmFd = openSharedMemory(&pShm);
    

    // Open or create the shared memory segment

    
    // Copy the content into a buffer
    char buffer[SHM_SIZE];
    memcpy(buffer, pShm, SHM_SIZE);

    DEBUG_PRINT("New order received: %s\n", buffer);

    // example of order {{1:3}, {2:2}, {5:4}, {6:6}, {12:2}}

    // Unmap and close the shared memory segment
    munmap(pShm, SHM_SIZE);
    close(shmFd);

    
    // Process the order TODO
    
}

typedef struct {
    int id;
    int aisle, row;
    int quantity;
    int aisleL, aisleR;
    sfVector2f *waypointsL, *waypointsR;
} ItemPath;

int extractIdQty(char *orderStr, int **idItemsOrder, int **qtyItemsOrder) {
    const char *ptr = orderStr;
    int nbItems = 0;
    int size = 1;
    
    CHECK_NULL(*idItemsOrder = malloc(size * sizeof(int)), "extractIdQty: allocation error");
    CHECK_NULL(*qtyItemsOrder = malloc(size * sizeof(int)), "extractIdQty: allocation error");
    

    while ((ptr = strchr(ptr, '{')) != NULL) {
        if (sscanf(ptr, "{%d:%d}",  &(*idItemsOrder)[nbItems], &(*qtyItemsOrder)[nbItems]) == 2){
            nbItems++;

            if (nbItems == size) {
                size *= 2;
                CHECK_NULL(*idItemsOrder = realloc(*idItemsOrder, size * sizeof(int)), "extractIdQty: reallocation error");
                CHECK_NULL(*qtyItemsOrder = realloc(*qtyItemsOrder, size * sizeof(int)), "extractIdQty: reallocation error");
            }
            DEBUG_PRINT("ID: %d, Quantity: %d\n", (*idItemsOrder)[nbItems-1], (*qtyItemsOrder)[nbItems-1]);
            
        }
        ptr++; // Avancer pour éviter de boucler sur le même caractère
    }

    return nbItems;
}
    
    
ItemPath *load_inventory(const char *filepath, int nbItems, int *idItemsOrder, int *qtyItemsOrder) {
    json_t *root, *item;
    json_error_t error;
    size_t index;
    ItemPath *order = NULL;

    // Charger le fichier JSON
    root = json_load_file(filepath, 0, &error);
    CHECK_NULL(root, "Erreur de chargement du fichier JSON");

    // Vérifier que c'est un tableau
    if (!json_is_array(root)) {
        fprintf(stderr, "Erreur: le fichier JSON n'est pas un tableau\n");
        json_decref(root);
        return NULL;
    }

    order = malloc(nbItems * sizeof(ItemPath));
    if (!order) {
        perror("Erreur d'allocation mémoire");
        json_decref(root);
        return NULL;
    }

    // Parcourir le tableau JSON et stocker les données
    int j = 0;
    json_array_foreach(root, index, item) {
        int id = json_integer_value(json_object_get(item, "id"));

        for (int i = 0; i < nbItems; i++) {
            if (id == idItemsOrder[i]) {
                order[j].id = id;
                order[j].aisle = json_integer_value(json_object_get(item, "aisle"));
                order[j].row = json_integer_value(json_object_get(item, "row"));
                order[j].quantity = qtyItemsOrder[i];
                j++;
            }
        }
    }
    
    json_decref(root); // Libérer la mémoire JSON
    return order;


}

void constructAisle(ItemPath *order, int nbItems) {
    for (int i = 0; i < nbItems; i++) {
        switch (order[i].aisle)
        {
        case 1:
            order[i].aisleL = 1;
            order[i].aisleR = -1;
            break;
        case 2:
            order[i].aisleL = 2;
            order[i].aisleR = 1;
            break;
        case 3:
            order[i].aisleL = 3;
            order[i].aisleR = 2;
            break;
        case 4:
            order[i].aisleL = -1;
            order[i].aisleR = 3;
            break;
        default:
            break;
        }
    }
}

void constructWaypoint(ItemPath *order, int nbItems) {
    for (int i = 0; i < nbItems; i++) {
        if (order[i].aisleL != -1) {
            order[i].waypointsL = malloc(order[i].row * sizeof(sfVector2f));
            // order[i].waypointsL[0] = (sfVector2f) {LARGEUR_ENVIRONNEMENT - ECART_LONGUEUR - LARGEUR_ETAGERE - LARGEUR_BAC/2, (order[i].aisleL - 1) * (LARGEUR_ALLEE + LARGEUR_ETAGERE) + LARGEUR_ALLEE/2};

            // To finish 
        }    
    }

}

#ifdef TEST_ORDER
int main(int argc, char *argv[]){

    int *idItemsOrder = NULL;
    int *qtyItemsOrder = NULL;
    int nbItems = extractIdQty("{{1:3}, {2:2}, {6:6}, {12:2}, {15:4}}", &idItemsOrder, &qtyItemsOrder);
    

    ItemPath *order = load_inventory(PATH_INVENTORY, nbItems, idItemsOrder, qtyItemsOrder);
    constructAisle(order, nbItems);
    if (order) {
        for (int i = 0; i < nbItems; i++) {
            printf("ID: %d, Aisle: %d, Row: %d, Quantity: %d, aisleL: %d, aisleR: %d\n", order[i].id, order[i].aisle, order[i].row, order[i].quantity, order[i].aisleL, order[i].aisleR);
        }
    }
    free(idItemsOrder);
    free(qtyItemsOrder);
    free(order);
    return 0;

}

#endif