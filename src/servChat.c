#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "servFcnt.h"
#include "common.h"

#ifdef _WIN64
#include <winsock.h>

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(param) close(param)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

int setupChatServer()
{
#ifdef _WIN64

#else

    int windWSAError = 0;

#endif

    SOCKADDR_IN serveurChatAddr = {0};
    SOCKET sockServ;
    SOCKET sock;
    SOCKET SockTempServ;
    SOCKADDR_IN clientChatAddr;
    socklen_t sizeofClientChatAddr;
    int option = 1;

    if (!windWSAError)
    {
        serveurChatAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveurChatAddr.sin_family = AF_INET;
        serveurChatAddr.sin_port = htons(CHAT_PORT);

        sockServ = socket(AF_INET, SOCK_STREAM, 0);

        if (sockServ != INVALID_SOCKET)
        {
            if (verbose >= 1)
                printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", sockServ);

            sock = bind(sockServ, (SOCKADDR *)&serveurChatAddr, sizeof(serveurChatAddr));

            if (sock != SOCKET_ERROR)
            {
                if (verbose >= 1)
                {
                    printf("La socket Chat est valide et bind dans sockServ \n");
                    printf("Démarrage de lécoute du serveur chat... \n");
                }

                if (setsockopt(sock, 1, SO_REUSEADDR, (char *)&option, sizeof(option)) < 0)
                {
                    printf("Socket option don't work \n");
                }
                else
                {
                    if(verbose >= 0)printf("Set option to socket is OK \n");
                    SockTempServ = listen(sockServ, 5);

                    if (SockTempServ != SOCKET_ERROR)
                    {
                        if (verbose >= 1)
                        {
                            printf("le client s'est connecté ! \n");
                        }

                        sizeofClientChatAddr = sizeof(clientChatAddr);

                        SockTempServ = accept(sockServ, (struct sockaddr *)&clientChatAddr, &sizeofClientChatAddr);

                        if (SockTempServ != -1)
                        {
                            if (verbose >= 1)
                            {
                                printf("Le client chat est conecté ! \n");
                            }

                            socketConnectedChat = sockServ;

                            if (verbose >= 0)
                                printf("SocketConnectedChat = %d | sock = %d \n", socketConnectedChat, sockServ);
                        }
                        else
                        {
                            printf("Pas de connexion de client Chat \n");
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Erreur à la connexion du client Chat \n");
                        return -1;
                    }
                }
            }
            else
            {
                printf("Erreur au bind du socket Chat \n");
                return -1;
            }
        }
        else
        {
            printf("Erreur à la création du socket Chat \n");
            return -1;
        }
    }
    else
    {
        printf("Erreur windWSAError \n");
        return -1;
    }
    return 0;
}