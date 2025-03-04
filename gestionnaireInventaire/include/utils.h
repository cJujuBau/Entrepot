/* ------------------------------------------------------------------------ */
/*                            Serial rpi/ATmega                             */
/*                        Auteur : CHEVALIER Romain                         */
/*                            Date : 26-10-2024                             */
/* ------------------------------------------------------------------------ */


#ifndef __UTIL_H

#define __UTIL_H

/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */




/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*                      M A C R O - F O N C T I O N S                       */
/* ------------------------------------------------------------------------ */



#define CHECK_T(status, msg)                                                 \
  if (0 != (status))   {                                                     \
    fprintf(stderr, "pthread erreur : %s avec erreur n°%d\n", msg, status);  \
    exit (EXIT_FAILURE);                                                     \
  }

#define CHECK(status, msg)                                                   \
    if (-1 == (status)) {                                                    \
        perror(msg);                                                         \
        exit(EXIT_FAILURE);                                                  \
    }

#define CHECK_NULL(status, msg)                                              \
    if (NULL == (status)) {                                                  \
        perror(msg);                                                         \
        exit(EXIT_FAILURE);                                                  \
    }




#ifdef DEBUG
    #define DEBUG_PRINT(msg, ...) do {                                      \
        printf(msg, ##__VA_ARGS__);                                         \
    } while (0)
#else
    #define DEBUG_PRINT(msg, ...) // Ne fait rien si DEBUG n'est pas défini
#endif

    
#endif
