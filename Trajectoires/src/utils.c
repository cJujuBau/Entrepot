// filepath: /home/arnaud/Bureau/Entrepot/Entrepot/Trajectoires/utils.c
#include <utils.h>
#include <math.h>

float distance(sfVector2f posA, sfVector2f posB)
{
    return sqrt((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y));
}