
/* =============== DEPENDANCES =============== */


#include <stdio.h>
#include <ctype.h>
#include <string.h>


/* =============== CONSTANTES =============== */





/* =============== VARIABLES =============== */





/* =============== FONCTIONS =============== */


char * strToUpper(char *chaine)
{
    char * temp = chaine;

    int i = 0;
    for (i = 0 ; temp[i] != '\0' ; i++)
    {
        temp[i] = toupper(temp[i]);
    }

    return temp;
}