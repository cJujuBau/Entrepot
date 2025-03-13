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

int bac1isUsed = 0;
int bac2isUsed = 0;

OrdersElementary ordersElementaryGlobal = NULL;
ItemPath itemPathNULL = {-1, -1, -1, -1, -1, -1, -1, NULL, NULL};



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
    processOrder(buffer);
}

void processOrder(char *orderStr) {
    int *idItemsOrder = NULL;
    int *qtyItemsOrder = NULL;
    int nbItems = -1;

    // Construct the order
    nbItems =  extractIdQty(orderStr, &idItemsOrder, &qtyItemsOrder);
    ItemPath *order = load_inventory(PATH_INVENTORY, nbItems, idItemsOrder, qtyItemsOrder);
    
    if (order == NULL) {
        DEBUG_PRINT("Error: order is NULL\n");
        return;
    }

    attributeBac(order, nbItems);

    constructAisle(order, nbItems);
    constructWaypoint(order, nbItems);

        

    // Print the order
    
    // for (int i = 0; i < nbItems; i++) {
    //     DEBUG_PRINT("ID: %d, Aisle: %d, Row: %d, Bac: %d, Quantity: %d, aisleL: %d, aisleR: %d\n", order[i].id, order[i].rack, order[i].row, order[i].bac, order[i].quantity, order[i].aisleL, order[i].aisleR);

    //     if (order[i].aisleL != -1) {
    //         DEBUG_PRINT("Waypoint L: (%.2f, %.2f) (%.2f, %.2f)\n", order[i].waypointsL[0].x, order[i].waypointsL[0].y, order[i].waypointsL[1].x, order[i].waypointsL[1].y);
    //     }
    //     if (order[i].aisleR != -1) {
    //         DEBUG_PRINT("Waypoint R: (%.2f, %.2f) (%.2f, %.2f)\n", order[i].waypointsR[0].x, order[i].waypointsR[0].y, order[i].waypointsR[1].x, order[i].waypointsR[1].y);
    //     }
    // }
    

    int **idItemsOrderElementary = NULL;
    int nbOrdersElementary =  divideOrder(order, nbItems, &idItemsOrderElementary);

    // DEBUG_PRINT("Number of elementary orders: %d\n", nbOrdersElementary);
    // for (int i = 0; i< nbOrdersElementary; i++){
    //     DEBUG_PRINT("IDS of Constructed Elementary order %d: %d,", i, order[(idItemsOrderElementary)[i][0]].id);
        
    //     if ((idItemsOrderElementary)[i][1] == -1) DEBUG_PRINT("%d\n", idItemsOrderElementary[i][1]);
    //     else DEBUG_PRINT("%d\n", order[(idItemsOrderElementary)[i][1]].id);
    // }
    
    addOrdersToOrdersGlobal(idItemsOrderElementary, nbOrdersElementary, order);
   
    DEBUG_PRINT("Order treated, adding new %d elementary order\n", nbOrdersElementary);
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
                order[j].bac = -1;
                order[j].aisleL = -1;
                order[j].aisleR = -1;
                order[j].waypointsL = NULL;
                order[j].waypointsR = NULL;
                j++;
            }
        }
    }
    
    json_decref(root); // Libérer la mémoire JSON
    return order;
}

// Gerer l'attente des commandes
int attributeBac(ItemPath *order, int nbItems){
    static int bac = 0;

    bac = (bac == 1) ? 2 : 1;
    printf("Bac attribué: %d\n", bac);
    
    
    for (int i = 0; i < nbItems; i++) {
        order[i].bac = bac;
    }
    return bac;
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

            order[i].waypointsL[1].x = order[i].waypointsL[0].x + LARGEUR_ALLEE / 2;
            order[i].waypointsL[1].y = order[i].waypointsL[0].y;
        }   
        if (order[i].aisleR != -1){
            // waypoint for the right aisle
            order[i].waypointsR = malloc(2 * sizeof(sfVector2f));
            order[i].waypointsR[0].x = ECART_LONGUEUR + LARGEUR_ETAGERE + (MAX_AISLE - order[i].aisleR) * (LARGEUR_ETAGERE + LARGEUR_ALLEE) + LARGEUR_ALLEE / 2;
            order[i].waypointsR[0].y = (NB_ITEMS_PAR_ALLEE - order[i].row) * LONGUEUR_ETAGERE_PAR_ITEM + LONGUEUR_ETAGERE_PAR_ITEM / 2;

            order[i].waypointsR[1].x = order[i].waypointsR[0].x + LARGEUR_ALLEE /2;
            order[i].waypointsR[1].y = order[i].waypointsR[0].y;
        } 
    }

}

int divideOrder(ItemPath *order, int nbItems, int ***idItemsOrderElementary) {
    int nbOrdersElementary = 0;
    int sizeOrdersElementary = 1;
    
    // Allocation initiale du tableau de pointeurs
    *idItemsOrderElementary = malloc(sizeOrdersElementary * sizeof(int *));
    CHECK_NULL(*idItemsOrderElementary, "divideOrder : malloc(idItemsOrderElementary)");

    // Allocation du premier élément
    (*idItemsOrderElementary)[0] = malloc(sizeof(int) * 2);
    CHECK_NULL((*idItemsOrderElementary)[0], "divideOrder : malloc(idItemsOrderElementary[0])");

    int i = 0;
    int quantity = order[i].quantity;
    
    while (1) {
        // divide the order into elementary orders
        (*idItemsOrderElementary)[nbOrdersElementary][0] = i;
        --quantity;

        // if the quantity of the item is 0, we move to the next item 
        if (quantity == 0) {
            i++;
            if (i == nbItems){
                ++nbOrdersElementary; // If the last item is reached, increase number of orders before breaking
                break;
            } 
            quantity = order[i].quantity;
        }

        (*idItemsOrderElementary)[nbOrdersElementary][1] = i;
        --quantity;

        // if the quantity of the item is 0, we move to the next item   
        if (quantity == 0) {
            i++;
            if (i == nbItems){
                ++nbOrdersElementary; // If the last item is reached, increase number of orders before breaking
                break;
            } 
            quantity = order[i].quantity; // else we update the quantity of the item
        }


        // Reach the max of item per elementary order, create a new one
        ++nbOrdersElementary;

        // Increase size if necessary
        if (nbOrdersElementary == sizeOrdersElementary) {
            sizeOrdersElementary *= 2;
            *idItemsOrderElementary = realloc(*idItemsOrderElementary, sizeOrdersElementary * sizeof(int *));
            CHECK_NULL(*idItemsOrderElementary, "divideOrder : realloc(idItemsOrderElementary)");
        }
        
        // Allocate a new elementary order
        (*idItemsOrderElementary)[nbOrdersElementary] = malloc(sizeof(int) * 2);
        CHECK_NULL((*idItemsOrderElementary)[nbOrdersElementary], "divideOrder : malloc(idItemsOrderElementary[nbOrdersElementary])");

        (*idItemsOrderElementary)[nbOrdersElementary][0] = -1;
        (*idItemsOrderElementary)[nbOrdersElementary][1] = -1;

        
    }   

    return nbOrdersElementary;
}

void addOrdersToOrdersGlobal(int **idItemsOrderElementary, int nbOrdersElementary, ItemPath *order){
    if (ordersElementaryGlobal == NULL) {
        ordersElementaryGlobal = malloc(sizeof(struct OrdersElementary));
        CHECK_NULL(ordersElementaryGlobal, "addOrdersToOrdersGlobal : malloc(ordersElementaryGlobal)");

        ordersElementaryGlobal->nbOrdersElementary = 0;
        ordersElementaryGlobal->sizeOrdersElementary = 1;
        ordersElementaryGlobal->items = malloc(ordersElementaryGlobal->sizeOrdersElementary * sizeof(ItemPath *));
        CHECK_NULL(ordersElementaryGlobal->items, "addOrdersToOrdersGlobal : malloc(ordersElementaryGlobal->ItemPath)");
    }

    while (ordersElementaryGlobal->sizeOrdersElementary <= ordersElementaryGlobal->nbOrdersElementary + nbOrdersElementary) {
        ordersElementaryGlobal->sizeOrdersElementary *= 2;
        ordersElementaryGlobal->items = realloc(ordersElementaryGlobal->items, ordersElementaryGlobal->sizeOrdersElementary * sizeof(ItemPath*));
        CHECK_NULL(ordersElementaryGlobal->items, "addOrdersToOrdersGlobal : realloc(ordersElementaryGlobal->ItemPath)");
    }
    
    int currentIndex = 0;

    for (int i =0; i<nbOrdersElementary; i++){
        currentIndex = ordersElementaryGlobal->nbOrdersElementary + i;
        ordersElementaryGlobal->items[currentIndex] = malloc(2*sizeof(ItemPath));
        ordersElementaryGlobal->items[currentIndex][0] = order[idItemsOrderElementary[i][0]];
        
        if (idItemsOrderElementary[i][1] == -1) ordersElementaryGlobal->items[currentIndex][1] = itemPathNULL;
        else ordersElementaryGlobal->items[currentIndex][1] = order[idItemsOrderElementary[i][1]];
    }

    ordersElementaryGlobal->nbOrdersElementary += nbOrdersElementary;
    
}

void removeOrderFromOrdersGlobal(int index){
    if (ordersElementaryGlobal == NULL) return;

    if (index < 0 || index >= ordersElementaryGlobal->nbOrdersElementary) return;

    // free(ordersElementaryGlobal->items[index]);
    ordersElementaryGlobal->items[index] = NULL;

    for (int i = index; i < ordersElementaryGlobal->nbOrdersElementary - 1; i++) {
        ordersElementaryGlobal->items[i] = ordersElementaryGlobal->items[i + 1];
    }

    ordersElementaryGlobal->nbOrdersElementary--;
}

void printOrdersElementary(){
    DEBUG_PRINT("Number of elementary orders: %d\n", ordersElementaryGlobal->nbOrdersElementary);
    if (ordersElementaryGlobal) {
        for (int i = 0; i < ordersElementaryGlobal->nbOrdersElementary; i++) {
            DEBUG_PRINT("Elementary order %d: %d, %d\n", i, ordersElementaryGlobal->items[i][0].id, ordersElementaryGlobal->items[i][1].id);
            DEBUG_PRINT("Bac de la commande elementaire: %d, %d\n", ordersElementaryGlobal->items[i][0].bac, ordersElementaryGlobal->items[i][1].bac);
        }
    }
}

// free the memory allocated 
void freeOrdersElementary(){
    DEBUG_PRINT("Free memory\n");
    if (ordersElementaryGlobal) {
        for (int i = 0; i < ordersElementaryGlobal->nbOrdersElementary; i++) {

            free(ordersElementaryGlobal->items[i]);
            ordersElementaryGlobal->items[i] = NULL;
        }
        free(ordersElementaryGlobal->items);
        free(ordersElementaryGlobal);
    }
}



#ifdef TEST_ORDER

// ONLY FOR TESTING

int main(int argc, char *argv[]){

    // processOrder("{{1:3}, {2:2}, {6:6}, {12:2}, {15:4}}");
    // processOrder("{{1:3}, {2:2}, {6:6}, {12:2}, {15:4}}");
    // processOrder("{{1:3}, {2:2}, {6:6}, {12:2}, {15:4}}");


    processOrder("{{1:1}, {2:1}, {6:1}, {9:1}, {12:1}, {14:1}}");

    printOrdersElementary();

    removeOrderFromOrdersGlobal(1);
    printOrdersElementary();


    freeOrdersElementary();
    return 0;

}

#endif