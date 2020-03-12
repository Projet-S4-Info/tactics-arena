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

comm sendSrv;
sendSrv.flag = 1;
  
  int test;
  test = send(sock, (void *)&sendSrv, sizeof(sendSrv), 0);
  if(test != SOCKET_ERROR){
    printf("Message envoyé \n");
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
  printf("\nLancement de la créatoin du client...\n");
  /*
  * Setting up the socket for all systems
  */
  SOCKADDR_IN sockIn;
  SOCKET sock;
  if(!windWSAError){
    /*
    * Creating socket :
    * param 1 : Use TCP/IP
    * param 2 : Use with TCP/IP
    * param 3 : Protocole parameter (useless) -> 0
    */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /*
    * Check if the socket is correct
    */
    if(sock != INVALID_SOCKET){
      const char * servIP = malloc(sizeof(char) * MAX_BUFF_SIZE);
      printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", sock);
      /*
      * Initialising struct
      * Can change s_addr with given ip inet_addr("192.168.0.0")
      */
      servIP = setServIP();
      // printf("\n%s\n", servIP);
      sockIn.sin_addr.s_addr= inet_addr((char *)servIP);
      sockIn.sin_family = AF_INET;
      sockIn.sin_port = htons(PORT);
      int choixCli = 0;
      /*
      *
      *If client achieve connection
      */
      if(connect(sock, (SOCKADDR*)&sockIn, sizeof(sockIn)) != SOCKET_ERROR){
        printf("Connexion réussie à : %s sur le port : %d \n", inet_ntoa(sockIn.sin_addr), htons(sockIn.sin_port));

        t_personnage monpersoCli;
        monpersoCli.id = 1;
        sprintf(monpersoCli.nom, "Lucien");
        
        t_msgChat monMsg;
        monMsg.ident = 2;
        sprintf(monMsg.msg,"Client");
      
        char pseudoCli[128];
        flushMsg(pseudoCli);
        printf("Saisir votre pseudo : ");
        scanf("%s",pseudoCli);
        printf("\nVous vous appelez : %s", pseudoCli);
        sprintf(monMsg.pseudo,"%s",pseudoCli);
        
        
        // printf("L'id du perso est : %d \n", monpersoCli.id);
        // printf("Le nom du perso est : %s \n", monpersoCli.nom);

        printf("\nDébut de la communication : \n");
        
        printf("Press (1) start chat :\n");
        printf("Pess (2) send structure : \n");
        scanf("%d",&choixCli);
        switch(choixCli){
          case 1: startChat(sock,pseudoCli,(t_msgChat)monMsg);break;
          case 2: sendStruct(sock, (t_personnage)monpersoCli);break;
          case 3: sendmvt(sock);
          
        }

        
        printf("Fin de la communication \n");
      }
      else{
        printf("Impossble de se connecter au serveur... :( \n");
      }
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
