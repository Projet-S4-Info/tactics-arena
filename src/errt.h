#ifndef errt_h
#define errt_h
#include "common.h"
#define NUM_ERROR 4

char error_message[NUM_ERROR][STR_SHORT] = {"OK\n", "ERROR: Pointer is Null\n", "ERROR : SDL\n", "ERROR : Coordinates out of limits\n"};

#endif