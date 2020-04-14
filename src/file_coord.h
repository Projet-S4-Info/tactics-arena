#ifndef file_coord_h
#define file_coord_h
#include "struct.h"

typedef struct Element Element;
struct Element
{
    Coord nombre;
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
};

File *initialiser();
bool file_vide(File *f);
void enfiler(File *file, Coord nvNombre);
Coord defiler(File *file);
void afficherFile(File *file);
void detruire_file(File *file);

#endif