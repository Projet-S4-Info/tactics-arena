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





typedef struct
{
   int stockListen;
 
   pthread_t thread_changes;
}
listenThread_t;

listenThread_t threadChanges;


static void * fn_listenChanges (){
    listenChanges(socketConnected);
    return NULL;
}

int stopTcpSocketServ(int socketConnected){
  printf("Shutdown socketConnected ...\n");
  shutdown(socketConnected, 2);
  printf("Close socket : socketConnected...\n");
  closesocket(socketConnected);
  return 0;
}

int mouvement(){
  printf("Le perso bouge ! \n");
  return 0;
}

int attack(){
  printf("Le perso attaque ! \n");
  return 0;
}

int listenChanges(int socketConnected){

  int sockEr = 0;
  int stopFlags = 1;

  comm recvCli;
  recvCli.flag = 0;
  while(stopFlags == 1){


   sockEr = recv(socketConnected,(void *)&recvCli, sizeof(recvCli), 0);
   if(sockEr != SOCKET_ERROR){
     printf("Le flag est : %d", recvCli.flag);
     switch(recvCli.flag){
       case 1: attack();sockEr = SOCKET_ERROR;break;
       case 2: mouvement();sockEr = SOCKET_ERROR;break;
    }
     
   }
    printf("Le flag est : %d", recvCli.flag);
    if(recvCli.flag == -1 ){
      stopFlags = 0;
    }
  }
  printf("Test print socketError : %d \n", sockEr);
  printf("Saisir un truc \n");
  getchar();
  return 0;
}


int startTCPSocketServ(){
  #ifdef _WIN32
    /*
    * Change the cmd codepage
    * Create firewall rules
    */
    system("chcp 65001");
    system("netsh advfirewall firewall add rule name=\"Tactics\" protocol=TCP dir=in localport=50135 action=allow");
    system("netsh advfirewall firewall add rule name=\"Tactics\" protocol=TCP dir=out localport=50135 action=allow");
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


  printf("\nLancement de la créatoin du serveur...\n");
  logFlag += 1;
  /*
  * Setting up the socket for all systems
  */
  SOCKADDR_IN sockServIn = { 0 };
  int sock;
  int sockError;
  SOCKADDR_IN sockConnectedAddr;
  socklen_t sizeofSocketConnected;

  if(!windWSAError){
   
    /*
    * Initialising struct
    * Can change s_addr with given ip inet_addr("192.168.0.0") or INADDR_ANY
    */
    sockServIn.sin_addr.s_addr=htonl(INADDR_ANY) ;
    sockServIn.sin_family = AF_INET;
    sockServIn.sin_port = htons (PORT);
    /*
    * Creating socket :
    * param 1 : Use TCP/IP
    * param 2 : Use with TCP
    * param 3 : Protocole parameter (useless) -> 0
    */
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET){
  
      printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", sock);
      
      /*
      *bind info to the socket
      */
      sockError = bind(sock, (SOCKADDR*)&sockServIn, sizeof(sockServIn));
      /*
      * Check if the socket is correct
      */
      if(sockError != SOCKET_ERROR){
        printf("\nDémarrage du serveur... \n");
        /*
        * Starting to connect
        * (max number of connection 5)
        */
        getLocalIP();
        sockError = listen(sock,5);
        if(sockError != SOCKET_ERROR){
          printf("\nEn attente de la connexion d'un client...\n");
          getLocalIP();
          sizeofSocketConnected = sizeof(sockConnectedAddr);
          socketConnected = accept(sock, (struct  sockaddr  *)&sockConnectedAddr, &sizeofSocketConnected);
          if(socketConnected != SOCKET_ERROR){
            
            int choixServ = 0;
            
            printf("\nConnexion établie avec le client !\n");
            printf("\nChargement de la partie... \n");

            t_msgChat monMsg;
            monMsg.ident = 2;
            sprintf(monMsg.msg,"Client");
          
            char pseudoCli[128];
            flushMsg(pseudoCli);
            printf("Saisir votre pseudo : ");
            scanf("%s",pseudoCli);
            printf("\nVous vous appelez : %s", pseudoCli);
            sprintf(monMsg.pseudo,"%s",pseudoCli);
            

            printf("\nPress (1) start chat :");
            printf("\nPress (2) send structure : ");
            printf("\nPress (3) start silent chat: ");
            scanf("%d",&choixServ);
            switch(choixServ){
              // case 1: startChat(socketConnected);break;
             // case 2 : sendStruct(socketConnected, (t_personnage)monpersoServ);break;
              case 3 : silentChat(socketConnected, pseudoCli,(t_msgChat)monMsg);break;
              case 4 : stopTcpSocketServ(socketConnected);break;
              case 5 : pthread_create (&threadChanges.thread_changes, NULL, fn_listenChanges, NULL);
            }

            // if(recv(socketConnected,(void *)&monpersoServ, sizeof(monpersoServ), 0) != SOCKET_ERROR){
              //   printf("Votre perso a été modifié ! \n");
              //   printf("l'id du perso est maintenant : %d \n", monpersoServ.id);
              //   printf("Le nom du perso est maintenant : %s \n", monpersoServ.nom);
              // }

            printf("Fin de la partie :( \n");
            /* Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens) */
          }

         // system("netsh advfirewall firewall delete rule name=\"Tactics\"");
        }
        else{
          printf("\nUn problème est survenu lors de la connexion du client :( \n");
          return 4;
        }
      }
      else{
        printf("\nUn problème est survenu lors de la liaison avec le client :( \n");
        return 3;
      }
    }
    else{
      printf("\nUn problème est survenu lors de la création de la socket :( \n");
      return 2;
    }
  }
  else{
    printf("Un problème est survenu avec Windows :( \n");
    return 1;
  }
  getchar();
  return 0;
}