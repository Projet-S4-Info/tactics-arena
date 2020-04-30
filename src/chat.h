#ifndef chat_h
#define chat_h
#include "struct.h"

extern chat_t chat;

err_t init_chat(chat_t * c);

err_t nouveau_Msg(chat_t *c, char msg[STR_LONG]);

void startChat(chat_t chat,int size, int socket);


#endif