#ifndef init_classes_h
#define init_classes_h
#include "common.h"

err_t init_berserker(Class * c);
err_t init_ranger(Class * c);
err_t init_goliath(Class * c);
err_t init_mage(Class * c, Ability movesets[3][NUM_AB]);
err_t init_valkyrie(Class * c);
err_t init_angel(Class * c);

#endif