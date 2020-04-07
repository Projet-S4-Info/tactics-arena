#ifndef common_h
#define common_h

#define STR_SHORT 50
#define STR_LONG 256
#define NUM_ERROR 5
#define MAXRANGE 101
#define taille 30

typedef enum error_type {OK, POINTER_NULL, SDL_ERROR, COORD_OUT_OF_BOUND, INIT_COORD_ERR, CLI_OK, SERV_OK,CLI_ERROR, SERV_ERROR, SEND_OK, SEND_ERROR, RECV_OK} err_t;

typedef enum {FALSE, TRUE} bool;

extern bool verbose;

#endif
