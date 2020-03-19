#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "Socket_Server.h"



/*
* If program run on Windows
*/
#ifdef _WIN32
  #include <winsock2.h>
  /*
  * Needed  non-existent type with winsock2
  */
  typedef int socklen_t;
/*
* Else if program run on Linux
*/
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  /*
  * Avoid differences to close a socket
  * between socket.h and winsock2.h
  * Adding missing definitions with socket.h
  */
  #define closesocket(param) close(param)
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1
  /*
  * Adding missing types with socket.h
  */
  typedef int SOCKET;
  typedef struct sockaddr_in SOCKADDR_IN;
  typedef struct sockaddr SOCKADDR;
#endif



char monStr[80];




void getLocalIP(){
  char monIP[81];
  char s[81];

  FILE *fic;
  fic=fopen(".test.txt", "r" );
   if( fic == NULL ){
      printf( "The file was not opened\n" );
    }
   else
   {
      /*
      * Get ip from the file named fic
      */
     #ifdef _WIN32
      fgets(s,sizeof(s),fic);
    #else
      fscanf(fic,"%s",s);   // C4996
    #endif
      int cpt = 0;
  
      for(int i = 0; s[i]; i++){
        if(((int)s[i] >= 46) && ((int)s[i] <= 57)){
          monIP[cpt] = s[i];
          cpt++;
        }
      }
      printf("L'ip du serveur est : %s", monIP);
    }
}

const char * setServIP(){
  char * servIP = malloc(sizeof(char) * MAX_BUFF_SIZE);
  printf("Saisir l'addresse IP du serveur : ");
  scanf(" %s", servIP);
  // printf("\n%s\n", servIP);
  return servIP;
}

char * flushMsg(char monMsg[MAX_BUFF_SIZE]){
  // printf("\nMon MSG before flush : %s", monMsg);
    for(int i = 0; monMsg[i]; i++){
      monMsg[i] = ' ';
    }
  // printf("\nMon MSG after flush : %s", monMsg);
  return monMsg;
}

int sendStruct(int socket, t_personnage monperso){
  int sockSendError;

  sockSendError = send(socket, (void *)&monperso, sizeof(monperso), 0);
  if(sockSendError == SOCKET_ERROR){
    printf("Impossible d'envoyer la structure... \n");
  }
  else{
    printf("Structure envoyée ! ");
  }
  printf("Press key to continue...");
  getchar();
  return 0;
}

const char * realStr(){

  printf("Saisir votre message : ");
  scanf(" %[^\n]", monStr);

  printf("\n votre message : %s \n", monStr);

  return monStr;
}

void sendMsg(int socket, char pseudo[128], t_msgChat monMsg){
  int sockCli;
  char buffer[MAX_BUFF_SIZE];
  flushMsg(buffer);
  
  printf("Saisir votre message : ");
  scanf(" %[^\n]", buffer);

  printf("\nVotre message : %s \n", buffer);
  //printf("Buffer size sendMSG: %lu (%lu)", strlen(buffer), sizeof(buffer));
  sprintf(monMsg.msg,"%s",buffer);
  sprintf(monMsg.pseudo,"%s",pseudo);

  //printf("\nDEBUG SENDMSG : monMsg.msg:  %s", monMsg.msg);
  sockCli = send(socket,(void *)&monMsg, sizeof(monMsg), 0);
  if(sockCli != SOCKET_ERROR){
    printf("Message envoyé avec succes ! \n");
  }
  else{
    printf("Send MSG error ... \n");
  }
}


void startChat(int sock, char pseudo[128], t_msgChat monMsg){

    sendMsg(sock,pseudo,monMsg);
    silentChat(sock,pseudo,(t_msgChat)monMsg);
}

void silentChat(int sock, char pseudo[128], t_msgChat monMsg){

  time_t seconds;
  time(&seconds);
  
  // printf("\nsize of msgSlilentCHat.msg : %lu (%lu) \n", strlen(msgSilentChat.msg), sizeof(msgSilentChat.msg));


  while(1 && ((time(NULL) - seconds)  != (1 *60))){
    
    int msgRcv;
    msgRcv = recv(sock,(void *)&monMsg, sizeof(monMsg), 0);
    
    if(msgRcv != SOCKET_ERROR){
      printf("\nVous avez un nouveau message ! \n");
      printf("\nTest pseudo : %s\n", pseudo);
      printf("%s : %s\n",monMsg.pseudo, monMsg.msg);
      // printf("\nSize of msgSlilentCHat.msg : %lu (%lu) \n", strlen(msgSilentChat.msg), sizeof(msgSilentChat.msg));
      time(&seconds);
      sleep(1);
      flushMsg(monMsg.msg);

    }
      startChat(sock,pseudo,(t_msgChat)monMsg);
      // printf("Attente : ");
      // printf("%ld \n", (time(NULL) - seconds));
      sleep(1);
      flushMsg(monMsg.msg);


  }
}

