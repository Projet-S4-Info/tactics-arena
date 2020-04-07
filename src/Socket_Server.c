#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
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
  /*
  * Avoid differences to close a socket
  * between socket.h and winsock2.h
  * Adding missing definitions with socket.h
  * Adding missing types with socket.h
  */
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1
  #define closesocket(param) close(param)
  typedef int SOCKET;
  typedef struct sockaddr_in SOCKADDR_IN;
  typedef struct sockaddr SOCKADDR;
#endif

int socketConnected = 0;

unsigned int logFlag = 0;
char pseudoCli[128];
int socketConnectedSrv = 0;


err_t stopTcpSocketServ(int socketConnected){
  printf("Shutdown socketConnected ...\n");
  shutdown(socketConnected, 2);
  printf("Close socket : socketConnected...\n");
  closesocket(socketConnected);
  return 0;
}




err_t startTCPSocketServ(){
  #ifdef __WIN64
    /*
    * Change the cmd codepage
    * Create firewall rules
    */
    system("chcp 65001");
    system("netsh advfirewall firewall add rule name=\"Tactics\" protocol=TCP dir=in localport=3555 action=allow");
    system("netsh advfirewall firewall add rule name=\"Tactics\" protocol=TCP dir=out localport=3555 action=allow");
    system("ipconfig | findstr /r \"IPv4.*192\" > .test.txt");
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
    system("ifconfig | grep \"inet 1[97]2.*\" | sed \"s/netmask.*//g\" | sed \"s/inet//g\" > .test.txt");
    int windWSAError= 0;
  #endif


  if(verbose)printf("\nLancement de la créatoin du serveur...\n");
  logFlag = 1;
  /*
  * Setting up the socket for all systems
  */
  SOCKADDR_IN serveurAddr = { 0 };
  int sock;
  int sockError;
  SOCKADDR_IN clientAddr;
  socklen_t sizeofSocketConnected;

  t_user infoClient;
  infoClient.id = 0;
  sprintf(infoClient.pseudo ,"PasDeCli");

  if(!windWSAError ){
   
    /*
    * Initialising struct
    * Can change s_addr with given ip inet_addr("192.168.0.0") or INADDR_ANY
    */
    serveurAddr.sin_addr.s_addr=htonl(INADDR_ANY) ;
    serveurAddr.sin_family = AF_INET;
    serveurAddr.sin_port = htons (PORT);
    /*
    * Creating socket :
    * param 1 : Use TCP/IP
    * param 2 : Use with TCP
    * param 3 : Protocole parameter (useless) -> 0
    */
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET){
  
      if(verbose)printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", sock);
      sleep(1);
      logFlag = 2;
      
      /*
      *bind info to the socket
      */
      sockError = bind(sock, (SOCKADDR*)&serveurAddr, sizeof(serveurAddr));
      /*
      * Check if the socket is correct
      */
      if(sockError != SOCKET_ERROR){
        if(verbose)printf("\nDémarrage du serveur... \n");
        /*
        * Starting to connect
        * (max number of connection 5)
        */
        getLocalIP();
        if(verbose)printf("LIP DU SERV EST %s", monIP);
        sockError = listen(sock,5);
        if(sockError != SOCKET_ERROR){
          if(verbose)printf("\nEn attente de la connexion d'un client...\n");
          sleep(1);
          logFlag = 3;
          getLocalIP();
          sizeofSocketConnected = sizeof(clientAddr);
          socketConnected = accept(sock, (struct  sockaddr  *)&clientAddr, &sizeofSocketConnected);
          if(socketConnected != SOCKET_ERROR){
            
            int choixServ = 0;
            
            if(verbose)printf("\nConnexion établie avec le client !\n");
            sleep(1);
            logFlag = 4;

            socketConnectedSrv = socketConnected;
            if(verbose)printf("socketConnectedCli = %d\n", socketConnectedSrv);

            if(recep(&infoClient, sizeof(infoClient), socketConnectedSrv) == 1){
              if(verbose)printf("\nid client = %d | pseudo client = %s\n", infoClient.id, infoClient.pseudo);
              sprintf(pseudoCli, "%s s'est connecté !", infoClient.pseudo);
              if(verbose)printf("SocketServer pseudoCli : %s\n", pseudoCli);
              logFlag = 5; 
            }
            
            
            printf("\nChargement de la partie... \n");

            t_msgChat monMsg;
            monMsg.ident = 2;
            sprintf(monMsg.msg,"Client");
            if(isPseudoValid == 1){
              if(verbose)printf("\nVous vous appelez : %s", pseudoUser);
              sprintf(monMsg.pseudoChat,"%s",pseudoUser);
            }else{
              printf("\nLe pseudo n'est pas mis \n");
            }
            






          }
          /*-- Commande pour fermer le firewall sur windows --*/
         // system("netsh advfirewall firewall delete rule name=\"Tactics\"");
        }
        else{
          printf("\nUn problème est survenu lors de la connexion du client :( \n");
          return SERV_ERROR;
        }
      }
      else{
        printf("\nUn problème est survenu lors de la liaison avec le client :( \n");
        return SERV_ERROR;
      }
    }
    else{
      printf("\nUn problème est survenu lors de la création de la socket :( \n");
      return SERV_ERROR;
    }
  }
  else{
    printf("Un problème est survenu avec Windows :( \n");
    return SERV_ERROR;
  }
  return SERV_OK;
}