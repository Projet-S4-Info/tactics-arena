/**
 * \file servFcnt.c
 * \brief All the function used by the server or the client
 * \details Contains all the function for server and client 
 * \author Lucien Chauvin
 * \version 0.1.0
 * \date 18/03/2020
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "servFcnt.h"
#include "common.h"



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
char monIP[85];

int nbPlayer = 0;
int socketConnected = 0;



/**
 * \fn void getLocalIP(void)
 * \return void
 * \brief Function to get the local ip of the server
*/

void getLocalIP(){
  
  char s[81];
  FILE *fic;
  system("ifconfig | grep \"inet 1[97]2.*\" | sed \"s/netmask.*//g\" | sed \"s/inet//g\" > .test.txt");

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
    if(verbose)printf("L'ip du serveur est : %s", monIP);
  }
}

/**
 * \fn const char * setServIP(void)
 * \return const char *
 * \brief Function to set the server IP address to a const char * (used for debugging)
 */


const char * setServIP(){
  char * servIP = malloc(sizeof(char) * MAX_BUFF_SIZE);
  printf("Saisir l'addresse IP du serveur : ");
  scanf(" %s", servIP);
  // printf("\n%s\n", servIP);
  return servIP;
}

/**
 * \fn char * flushMsg(char msg[])
 * \return char *
 * \brief Function to flush and clean given string
*/


char * flushMsg(char monMsg[MAX_BUFF_SIZE]){
  // printf("\nMon MSG before flush : %s", monMsg);
    for(int i = 0; monMsg[i]; i++){
      monMsg[i] = ' ';
    }
  // printf("\nMon MSG after flush : %s", monMsg);
  return monMsg;
}

/**
 * \fn err_t sendStruct(void * structure, int size, int socket)
 * \return err_t SEND_OK or SEND_ERROR
 * \brief Generic function to send a structur to a client or a server
*/

err_t sendStruct(void * structure, int size, int socket){
  int sockSendError;

  sockSendError = send(socket,structure, size, 0);
  if(sockSendError != SOCKET_ERROR){
    return OK;
  }else{
    return SEND_ERROR;
  }
}

/**
 * \fn err_t sendMsg(int socket, char pseudo[], t_msgChat monMsg)
 * \return err_t SEND_OK or SEND_ERROR
 * \brief function to send a message to a client or a server
*/

err_t sendMsg(int socket, char pseudo[128], t_msgChat monMsg){
  int sockCli;
  char buffer[MAX_BUFF_SIZE];
  flushMsg(buffer);
  
  printf("Saisir votre message : ");
  scanf(" %[^\n]", buffer);

  printf("\nVotre message : %s \n", buffer);
  //printf("Buffer size sendMSG: %lu (%lu)", strlen(buffer), sizeof(buffer));
  sprintf(monMsg.msg,"%s",buffer);
  sprintf(monMsg.pseudoChat,"%s",pseudo);

  //printf("\nDEBUG SENDMSG : monMsg.msg:  %s", monMsg.msg);
  sockCli = send(socket,(void *)&monMsg, sizeof(monMsg), 0);
  if(sockCli != SOCKET_ERROR){
    if(verbose)printf("Message envoyé avec succes ! \n");
    return OK;
  }
  else{
    printf("Send MSG error ... \n");
    return SEND_ERROR;
  }
}

/**
 * \fn void startChat(int socket, char pseudo[], t_msgChat monMsg)
 * \return void
 * \brief Function to start the chat
*/

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
      printf("%s : %s\n",monMsg.pseudoChat, monMsg.msg);
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

/**
 * \fn err_t recep(void * container, int size, int socket)
 * \return err_t RECV_OK 
 * \brief Generic function to receive structures
*/

void * recep(void * container, int size, int socket){
  if(verbose)printf("bienvenue dans recep \n");
  int flag = 0;
  while(flag == 0){
    if(recv(socket,container, size, 0) > -1){
      if(verbose)printf("struct reçue");
      flag = 1;
    }
  }
  return container;
}