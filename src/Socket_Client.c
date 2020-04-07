#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "Socket_Server.h"
#include "struct.h"
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
* Else if program run on Unix
*/
#else

  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
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
int socketConnectedCli = 0;


err_t stopTCPSocketCli(int socketCli){
  
  if(verbose)printf("Fermeture du socket Client ... \n");
  shutdown(socketCli, 2);
  closesocket(socketCli);
  return CLI_OK;
}


err_t startTCPSocketCli(int socketCli){

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

  
  const char * servIP = malloc(sizeof(char) * 85);

  t_user infoMoi;
    infoMoi.id = 111;
    sprintf(infoMoi.pseudo,"LucienCh2424");
      
  if(!windWSAError){
    servIP = ipSrv;
    if(verbose)printf("\n%s\n", servIP);
  
  /*---------- Initialisation des structures pour les sockets ----*/
    SOCKADDR_IN sockIn;
    SOCKET sock;
    sockIn.sin_addr.s_addr= inet_addr(servIP);
    sockIn.sin_family = AF_INET;
    sockIn.sin_port = htons(PORT);
  /*-------------------------------------------------------------*/
    
    if(verbose)printf("\nLancement de la création du client...\n");
    
    //-- Création de la socket (IPv4, TCP, 0)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock != INVALID_SOCKET){
      if(verbose)printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", sock);

      // -- Tentative de connection vers le serveur
      if(connect(sock, (SOCKADDR*)&sockIn, sizeof(sockIn)) != SOCKET_ERROR){
        if(verbose)printf("Connexion réussie à : %s sur le port : %d \n", inet_ntoa(sockIn.sin_addr), htons(sockIn.sin_port));
        socketConnectedCli = sock;
        if(verbose)printf("\nVous vous appelez : %s", pseudoUser);
        sprintf(infoMoi.pseudo, "%s", pseudoUser);
        if(verbose)printf("\nDébut de la communication : \n");
        if(verbose)printf("socketConnectedCli = %d\n", socketConnectedCli);

        sendStruct(&infoMoi, sizeof(infoMoi), socketConnectedCli);
        if(verbose)printf("Conexion établie sans soucis fermeture de la fonction... \n");
        return CLI_OK;
      }
      else{
        printf("Impossble de se connecter au serveur... :( \n");
        return CLI_ERROR;
      }
     
      
    }
    else{
      printf("\nLa socket est invalide :( \n");
      return CLI_ERROR;
    }
  }
  else{
    printf("\nImpossible de créer une socket :( \n");
    return CLI_ERROR;
  }
}
