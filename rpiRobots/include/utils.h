/* ------------------------------------------------------------------------ */
/*                                 Utils.h                                  */
/*                        Author: CHEVALIER Romain                          */
/*                            Date: 26-10-2024                              */
/* ------------------------------------------------------------------------ */


#ifndef __UTIL_H
#define __UTIL_H 1



/* ------------------------------------------------------------------------ */
/*                        M A C R O - F U N C T I O N S                     */
/* ------------------------------------------------------------------------ */



#define CHECK_T(status, msg)                                                        \
    if (0 != (status))   {                                                          \
        fprintf(stderr, "pthread error: %s with error number %d\n", msg, status);   \
        exit (EXIT_FAILURE);                                                        \
    }

#define CHECK(status, msg)                                                          \
        if (-1 == (status)) {                                                       \
                perror(msg);                                                        \
                exit(EXIT_FAILURE);                                                 \
        }

#define CHECK_NULL(status, msg)                                                     \
        if (NULL == (status)) {                                                     \
                perror(msg);                                                        \
                exit(EXIT_FAILURE);                                                 \
        }




#ifdef DEBUG
        #define DEBUG_PRINT(msg, ...) do {                                          \
                printf(msg, ##__VA_ARGS__);                                         \
        } while (0)
#else
        #define DEBUG_PRINT(msg, ...) // Does nothing if DEBUG is not defined
#endif

        
#endif

