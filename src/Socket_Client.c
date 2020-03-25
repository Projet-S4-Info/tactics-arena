#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "Socket_Server.h"
#include "struct.h"

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
* Else if program run on Unix
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

int socketCli = 0;

int sendmvt(int sock){

  int userch = 0;
  int stopF = 0;
  int test;

  comm sendSrv;
  sendSrv.flag = 1;

  

  while(stopF != -1){
    printf("Press (1) to attack : \n");
    printf("Press (2) to moove : \n");
    printf("Press (3) to stop all : \n");
    scanf("%d",&userch);
    switch (userch){
    case 1 : sendSrv.flag = 1;break;
    case 2 : sendSrv.flag = 2;break;
    case 3 : sendSrv.flag = -1;stopF = -1;break;
    }
  
    test = send(sock, (void *)&sendSrv, sizeof(sendSrv), 0);
    if(test != SOCKET_ERROR){
      printf("Message envoyé \n");
    }
  }
  return 0;
}


int startTCPSocketCli(int socketCli){

  #ifdef _WIN32
    /*
    * Change the cmd codepage
    */
    system("chcp 65001");
    system("cls");
    WSADATA WSAData;
    /*
    * Creating var to manage errors
    */
    int windWSAError;
    /*
    * WSAStratup Initialising winsock2 library
    * return 0 if there is no problems
    */
    windWSAError = WSAStartup(MAKEWORD(2,2), &WSAData);
  #else
    int windWSAError= 0;
  #endif

  int choixCli = 0;
  
  const char * servIP = malloc(sizeof(char) * 85);
  
  

  t_user infoMoi;
    infoMoi.id = 111;
    sprintf(infoMoi.pseudo,"LucienCh2424");

  t_msgChat monMsg;
        monMsg.ident = 2;
        sprintf(monMsg.msg,"Client");
      
  if(!windWSAError){
  servIP = ipSrv;
  printf("\n%s\n", servIP);
  
  /*---------- Initialisation des structures pour les sockets ----*/
  
  SOCKADDR_IN sockIn;
  SOCKET sock;
  sockIn.sin_addr.s_addr= inet_addr((char *)servIP);
  sockIn.sin_family = AF_INET;
  sockIn.sin_port = htons(PORT);

  /*-------------------------------------------------------------*/
    printf("\nLancement de la créatoin du client...\n");
    
    //-- Création de la socket (IPv4, TCP, 0)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock != INVALID_SOCKET){
      printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", sock);

      // -- Tentative de connection vers le serveur
      if(connect(sock, (SOCKADDR*)&sockIn, sizeof(sockIn)) != SOCKET_ERROR){
        printf("Connexion réussie à : %s sur le port : %d \n", inet_ntoa(sockIn.sin_addr), htons(sockIn.sin_port));

        
        printf("\nVous vous appelez : %s", pseudoUser);
        sprintf(monMsg.pseudoChat,"%s",pseudoUser);
        sprintf(infoMoi.pseudo, "%s", pseudoUser);

        printf("\nDébut de la communication : \n");
        sendStruct(sock, (t_user)infoMoi);
        
        printf("Press (1) start chat :\n");
        //printf("Pess (2) send structure : \n");
        printf("Press (3) send mvt : \n");
        scanf("%d",&choixCli);
        switch(choixCli){
          case 1: startChat(sock,pseudoUser,(t_msgChat)monMsg);break;
          //case 2: sendStruct(sock, (t_user)infoMoi);break;
          case 3: sendmvt(sock);
          
        }
        
        printf("Fin de la communication \n");
      }
      else{
        printf("Impossble de se connecter au serveur... :( \n");
      }
      printf("\nFermeture de la socket... \n");
      shutdown(sock, 2);
      closesocket(sock);
    }
    else{
      printf("\nLa socket est invalide :( \n");
    }
  }
  else{
    printf("\nImpossible de créer une socket :( \n");
  }

  getchar();
  return 0;
}
