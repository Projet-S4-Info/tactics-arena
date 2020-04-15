#ifndef common_h
#define common_h

#define STR_SHORT 50
#define STR_LONG 256
#define NUM_ERROR 8
#define MAXRANGE 101
#define taille 30
#define _NB_MAX_ChAT_ 10


typedef enum error_type {OK, POINTER_NULL, SDL_ERROR, COORD_OUT_OF_BOUND, STR_TOO_LONG, INIT_COORD_ERR, CLI_ERROR, SERV_ERROR, SEND_ERROR} err_t;

typedef enum {FALSE, TRUE} bool;

extern bool verbose;

extern int isChatActive;

extern char error_message[NUM_ERROR][STR_LONG];

extern char chatTab[STR_LONG][_NB_MAX_ChAT_];

char * strToUpper(char *chaine);

#endif
