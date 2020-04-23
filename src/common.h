#ifndef common_h
#define common_h

#define STR_SHORT 50
#define STR_LONG 256
#define NUM_ERROR 10
#define MAXRANGE 256
#define taille 30
#define _NB_MAX_CHAT_ 10
#define _NB_MAX_ANIM_ 13

typedef enum error_type {OK, POINTER_NULL, SDL_ERROR, COORD_OUT_OF_BOUND, STR_TOO_LONG, INIT_COORD_ERR, CLI_ERROR, SERV_ERROR, SEND_ERROR, ALREADY_IN_CACHE} err_t;

typedef enum {FALSE, TRUE} bool;

extern int verbose;

extern int isChatActive;
extern int isAServer;
extern int serverStatus;
extern int changesChat;
extern int nbPlayer;

typedef struct{
    int isServerStartGame;
    char mapNameGame[50];
}serverStatus_t;

extern char error_message[NUM_ERROR][STR_LONG];

extern char pseudoUser[50];
extern char mapMultiSelected[50];

char * strToUpper(char *chaine);

#endif
