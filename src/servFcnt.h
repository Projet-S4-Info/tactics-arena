#ifndef servFcnt_h
#define servFcnt_h
#define MAX_BUFF_SIZE 1024
#define PORT 3555
#include "struct.h"
#include "common.h"





typedef struct msgChat{
  int ident;
  char pseudoChat[64];
  char msg[MAX_BUFF_SIZE];
}t_msgChat;

extern unsigned int logFlag;
extern int isPseudoValid;
extern int isInfoJoinSet;
extern char monIP[85];
extern char ipSrv[85];
extern char MapMultiSelected[50];
extern char pseudoClient[128];
extern int socketConnected;

err_t startTCPSocketServ();
err_t startTCPSocketCli();
err_t stopTCPSocketServ(int socketConnected);
err_t stopTCPSocketCli(int socketConnected);
int listenChanges(int socketConnected);
err_t sendStruct(void * structure, int size,int socket);
void sendPseudo(int sock,t_user info);
err_t sendMsg(int socket, char pseudo[128], t_msgChat monMsg);
const char * realStr();
void getLocalIP();
const char * setServIP();
char * flushMsg(char * monMsg);
void * recep(void * container, int size, int socket);
int recepChat(void * structure, int size, int socket);



#endif
