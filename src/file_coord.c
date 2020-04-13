#include <stdio.h>
#include <stdlib.h>
#include "file_coord.h"
#include "struct.h"

File *initialiser()
{
    File *f = malloc(sizeof(*f));
    f->premier = NULL;

    return f;
}


void enfiler(File *f, Coord nvNombre)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (f == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->nombre = nvNombre;
    nouveau->suivant = NULL;

    if (f->premier != NULL) /* La f n'est pas vide */
    {
        /* On se positionne ‡ la fin de la f */
        Element *elementActuel = f->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
    }
    else /* La f est vide, notre ÈlÈment est le premier */
    {
        f->premier = nouveau;
    }
}


Coord defiler(File *f)
{
    if (f == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Coord nombreDefile = {0,0};

    /* On vÈrifie s'il y a quelque chose ‡ dÈfiler */
    if (f->premier != NULL)
    {
        Element *elementDefile = f->premier;

        nombreDefile = elementDefile->nombre;
        f->premier = elementDefile->suivant;
        free(elementDefile);
    }

    return nombreDefile;
}

void afficherFile(File *f)
{
    if (f == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *element = f->premier;

    while (element != NULL)
    {
        printf("x : %d y : %d ", element->nombre.x, element->nombre.y);
        element = element->suivant;
    }

    printf("\n");
}

void detruire_file(File *f)
{
    free(f);
}
