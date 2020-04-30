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

int setupChatClient()
{
#ifdef _WIN32

    // Cange codepage CMD
    system("chcp 65001");
    system("cls");
    WSADATA WSAData;
    int windWSAError;
    windWSAError = WSAStartup(MAKEWORD(2, 2), &WSAData);
#else
    int windWSAError = 0;
#endif

    const char *servIP = malloc(sizeof(char) * 85);

    if (!windWSAError)
    {
        servIP = ipSrv;
        if (verbose >= 1)
            printf("\n%s\n", servIP);

        SOCKADDR_IN sockCliChatAddr;
        SOCKET sockCliChat;
        sockCliChatAddr.sin_addr.s_addr = inet_addr(servIP);
        sockCliChatAddr.sin_family = AF_INET;
        sockCliChatAddr.sin_port = htons(CHAT_PORT);
        int option = 1;

        sockCliChat = socket(AF_INET, SOCK_STREAM, 0);

        if (sockCliChat != INVALID_SOCKET)
        {
            if (verbose >= 1)
            {
                printf("La socket client chat numéro %d est validée ! \n", sockCliChat);
            }

            if (setsockopt(sockCliChat, 1, SO_REUSEADDR, (char *)&option, sizeof(option)) < 0)
            {
                printf("Socket option don't work \n");
            }
            else
            {
                if (connect(sockCliChat, (SOCKADDR *)&sockCliChatAddr, sizeof(sockCliChatAddr)) != 0)
                {
                    if (verbose >= 1)
                    {
                        printf("La connexion au serveru chat est établie ! \n");
                        printf("Connexion à %s sur le port %d \n", inet_ntoa(sockCliChatAddr.sin_addr), htons(sockCliChatAddr.sin_port));
                    }

                    socketConnectedChat = sockCliChat;

                    if (verbose >= 0)
                        printf("SocketConnectedChat = %d | sock = %d \n", socketConnectedChat, sockCliChat);
                }
                else
                {
                    printf("Erreur lors de la connexion au serveur chat \n");
                }
            }
        }
        else
        {
            printf("Erreur lors de la création de la socket chat client \n");
            return -1;
        }
    }
    else
    {
        printf("Erreur windWSAError chat client \n");
        return -1;
    }
    return 0;
}