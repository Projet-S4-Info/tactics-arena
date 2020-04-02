#ifndef common_h
#define common_h

#define STR_SHORT 50
#define STR_LONG 256
#define NUM_ERROR 5

typedef enum error_type {OK, POINTER_NULL, SDL_ERROR, COORD_OUT_OF_BOUND, INIT_COORD_ERR} err_t;

typedef enum {FALSE, TRUE} bool;

#endif
