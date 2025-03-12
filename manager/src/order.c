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
    processOdrer(buffer);
}

void processOdrer(char *orderStr) {
    int *idItemsOrder = NULL;
    int *qtyItemsOrder = NULL;
    int nbItems = -1;

    // Construct the order
    nbItems =  extractIdQty(orderStr, &idItemsOrder, &qtyItemsOrder);
    ItemPath *order = load_inventory(PATH_INVENTORY, nbItems, idItemsOrder, qtyItemsOrder);
    constructAisle(order, nbItems);
    constructWaypoint(order, nbItems);

    // Print the order
    if (order) {
        for (int i = 0; i < nbItems; i++) {
            DEBUG_PRINT("ID: %d, Aisle: %d, Row: %d, Quantity: %d, aisleL: %d, aisleR: %d\n", order[i].id, order[i].rack, order[i].row, order[i].quantity, order[i].aisleL, order[i].aisleR);

            if (order[i].aisleL != -1) {
                DEBUG_PRINT("Waypoint L: (%.2f, %.2f) (%.2f, %.2f)\n", order[i].waypointsL[0].x, order[i].waypointsL[0].y, order[i].waypointsL[1].x, order[i].waypointsL[1].y);
            }
            if (order[i].aisleR != -1) {
                DEBUG_PRINT("Waypoint R: (%.2f, %.2f) (%.2f, %.2f)\n", order[i].waypointsR[0].x, order[i].waypointsR[0].y, order[i].waypointsR[1].x, order[i].waypointsR[1].y);
            }
        }
    }

    // Free memory
    free(idItemsOrder);
    free(qtyItemsOrder);
    free(order);
}

// Extract quantity and id from the order string
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
            // DEBUG_PRINT("ID: %d, Quantity: %d\n", (*idItemsOrder)[nbItems-1], (*qtyItemsOrder)[nbItems-1]);
            
        }
        ptr++; 
    }

    return nbItems;
}
    
// Load the inventory from a JSON file only for the items in the order
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

    CHECK_NULL(order = malloc(nbItems * sizeof(ItemPath)), "Memory allocation error");
    

    // Parcourir le tableau JSON et stocker les données
    int j = 0;
    json_array_foreach(root, index, item) {
        int id = json_integer_value(json_object_get(item, "id"));

        for (int i = 0; i < nbItems; i++) {
            if (id == idItemsOrder[i]) {
                order[j].id = id;
                order[j].rack = json_integer_value(json_object_get(item, "rack"));
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
        // First rack only one aisle on the left
        if (order[i].rack == 1) {
            order[i].aisleL = MIN_AISLE;
            order[i].aisleR = -1;
        } else if (order[i].rack == NOMBRE_ETAGERES) { 
            
            // Last rack only one aisle on the right
            order[i].aisleL = -1;
            order[i].aisleR = MAX_AISLE;
        } else { 
            
            // Other racks have aisles on both sides
            order[i].aisleL = order[i].rack;
            order[i].aisleR = order[i].rack - 1;
        }
    }
}

void constructWaypoint(ItemPath *order, int nbItems) {
    for (int i = 0; i < nbItems; i++) {
        if (order[i].aisleL != -1) {
            // waypoint for the left aisle
            order[i].waypointsL = malloc(2 * sizeof(sfVector2f));
            order[i].waypointsL[0].x = ECART_LONGUEUR + LARGEUR_ETAGERE + (MAX_AISLE - order[i].aisleL) * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2;
            order[i].waypointsL[0].y = (NB_ITEMS_PAR_ALLEE - order[i].row) * LONGUEUR_ETAGERE_PAR_ITEM + LONGUEUR_ETAGERE_PAR_ITEM / 2;

            order[i].waypointsL[1].x = order[i].waypointsL[0].x + LARGEUR_ALLEE / 4;
            order[i].waypointsL[1].y = order[i].waypointsL[0].y;
        }   
        if (order[i].aisleR != -1){
            // waypoint for the right aisle
            order[i].waypointsR = malloc(2 * sizeof(sfVector2f));
            order[i].waypointsR[0].x = ECART_LONGUEUR + LARGEUR_ETAGERE + (MAX_AISLE - order[i].aisleR) * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2;
            order[i].waypointsR[0].y = (NB_ITEMS_PAR_ALLEE - order[i].row) * LONGUEUR_ETAGERE_PAR_ITEM + LONGUEUR_ETAGERE_PAR_ITEM / 2;

            order[i].waypointsR[1].x = order[i].waypointsR[0].x + LARGEUR_ALLEE /4;
            order[i].waypointsR[1].y = order[i].waypointsR[0].y;
        } 
    }

}

#ifdef TEST_ORDER

// ONLY FOR TESTING

int main(int argc, char *argv[]){

    int *idItemsOrder = NULL;
    int *qtyItemsOrder = NULL;
    int nbItems = extractIdQty("{{1:3}, {2:2}, {6:6}, {12:2}, {15:4}}", &idItemsOrder, &qtyItemsOrder);
    

    ItemPath *order = load_inventory(PATH_INVENTORY, nbItems, idItemsOrder, qtyItemsOrder);
    constructAisle(order, nbItems);
    constructWaypoint(order, nbItems);
    if (order) {
        for (int i = 0; i < nbItems; i++) {
            printf("ID: %d, Aisle: %d, Row: %d, Quantity: %d, aisleL: %d, aisleR: %d\n", order[i].id, order[i].rack, order[i].row, order[i].quantity, order[i].aisleL, order[i].aisleR);
            if (order[i].aisleL != -1) {
                printf("Waypoint L: (%.2f, %.2f) (%.2f, %.2f)\n", order[i].waypointsL[0].x, order[i].waypointsL[0].y, order[i].waypointsL[1].x, order[i].waypointsL[1].y);
            }
            if (order[i].aisleR != -1) {
                printf("Waypoint R: (%.2f, %.2f) (%.2f, %.2f)\n", order[i].waypointsR[0].x, order[i].waypointsR[0].y, order[i].waypointsR[1].x, order[i].waypointsR[1].y);
            }
        }
    }
    free(idItemsOrder);
    free(qtyItemsOrder);
    free(order);
    return 0;

}

#endif