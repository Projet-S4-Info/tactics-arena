#ifndef socket_server_h
#define socket_server_h
#define MAX_BUFF_SIZE 1024
#define PORT 3555



typedef struct User{
  int id;
  char pseudo[64];
} t_user;

typedef struct msgChat{
  int ident;
  char pseudoChat[64];
  char msg[MAX_BUFF_SIZE];
}t_msgChat;

extern unsigned int logFlag;
extern int isPseudoValid;
extern int isInfoJoinSet;
extern char pseudoCli[128];
extern char monIP[85];
extern char ipSrv[85];
extern char pseudoUser[50];

void startTCPSocketServ();
int startTCPSocketCli();
int stopTCPSocketServ(int socketConnected);
int listenChanges(int socketConnected);
int sendStruct(int socket, t_user monperso);
void sendMsg(int socket, char pseudo[128], t_msgChat monMsg);
const char * realStr();
void getLocalIP();
void startChat(int sock, char pseudo[128], t_msgChat monMsg);
const char * setServIP();
void silentChat(int sock, char pseudo[128], t_msgChat monMsg);
char * flushMsg(char * monMsg);


#endif
