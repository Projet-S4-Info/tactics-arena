#ifndef chat_h
#define chat_h
#include "struct.h"

err_t init_chat(chat_t * c);

err_t nouveau_Msg(chat_t *c, char msg[STR_LONG]);

#endif