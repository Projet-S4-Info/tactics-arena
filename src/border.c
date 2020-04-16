#include "deplacement.h"
#include "struct.h"
#include "grid.h"
#include "gameplay.h"
#include "display.h"

int actionZone(int posX, int posY, int actionRange, Coord coorTab[]){

    int cpt = 0;
    int h;

    //quart gauche haut
    h = posY;
    for(int x = (posX - actionRange) ; x <= posX; x++){
        if((x > 0) && (x < taille)){
            coorTab[cpt].x = x;
            coorTab[cpt].y = h;
            cpt++;
        }else if(x <= 0 && h >= 0){
            coorTab[cpt].x = 0;
            coorTab[cpt].y = h;
            cpt++;
        }
        if(h <= 0){
            h = 0;
        }else{
            h--;
        }
    }

    //quart droit haut
    h = posY;
    for(int x = (posX + actionRange) ; x >= posX; x--){
        if((x > 0) && (x < taille )){
            coorTab[cpt].x = x;
            coorTab[cpt].y = h;
            cpt++;
        }else if(x >= taille -1){
            coorTab[cpt].x = taille -1;
            coorTab[cpt].y = h;
            cpt++;
        }
        if(h <= 0){
            h = 0;
        }else{
            h--;
        }
    }

    //quart droite bas
    h = posY;
    for(int x = (posX + actionRange) ; x >= posX; x--){
        if((x > 0) && (x < taille)){
            coorTab[cpt].x = x;
            coorTab[cpt].y = h;
            cpt++;
        }else if(x >= taille -1){
            coorTab[cpt].x = taille -1;
            coorTab[cpt].y = h;
            cpt++;
        }
        if(h >= taille -1){
            h = taille -1;
        }else{
            h++;
        }
    }

    //quart gauche bas
    h=posY;
    for(int x = (posX - actionRange) ; x <= posX; x++){
        if((x > 0) && (x < taille)){
            coorTab[cpt].x = x;
            coorTab[cpt].y = h;
            cpt++;
        }else if(x <= 0){
            coorTab[cpt].x = 0;
            coorTab[cpt].y = h;
            cpt++;
        }
        if(h >= taille -1){
            h = taille -1;
        }else{
            h++;
        }
    }
    
    coorTab[cpt].x = -99;
    coorTab[cpt].y = -99;

    if(verbose){
        printf("nbcoord : %d\n", cpt-1);
        for(int i = 0; i < MAXRANGE; i++){
            printf(" tour %d :\nx = %d\ny = %d\n", i, coorTab[i].x, coorTab[i].y);
        }
    }

    return 1;
}


bool isInRange(Coord coorTab[], Coord attack){
    
    int cursY = attack.y;
    int touchLine = 0;
    for(int i = 0; coorTab[i].x!=-99; i++){
        int cursX=0;
        for(int j = 0; j <= cursY; j++){
            if(cursX == (coorTab[i].x + 1) && cursY == (coorTab[i].y + 1)){
                touchLine++;
                cursX++;
            }
            if(verbose)printf("test %d : %d | %d : %d \n", cursX,cursY,coorTab[i].x, coorTab[i].y);
            cursX++;
        }
        if(verbose)printf("\n");
    }
    return touchLine%2 == 1;
}

int setActionZone(Coord perso, int actionRange, Coord coorTab[]){
    int posX = perso.x;
    int posY = perso.y;
    int res = 0;

    if(posX == 30 && posY == 30){
        if(verbose)printf("posX = 30 && posY = 30");
        res = actionZone(posX-1,posY -1, actionRange, coorTab);
    }else if(posX == 0 && posY == 0){
        if(verbose)printf("posX = 0 && posY = 0");
        res = actionZone(posX,posY, actionRange, coorTab);
    }else if(posX == 0 && posY == 30){
        if(verbose)printf("posX = 0 && posY = 30\n");
        res = actionZone(posX,posY-1, actionRange, coorTab);
    }else if (posX == 30 && posY == 0){
       if(verbose)printf("posx = 30 && posY = 0\n");
        res = actionZone(posX -1, posY, actionRange, coorTab);
    }
    else{
        if(verbose)printf("Autres cas \n");
        res = actionZone(posX,posY, actionRange, coorTab);
    }

    return res;
}

Coord * setMovementZone(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[])
{
    int i,j,k,l = 0;
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    Coord b,c;

    for(i=0; i<_X_SIZE_; i++)
    {
        for(j=0; j<_Y_SIZE_; j++)
        {
            if(matrice[i][j]!=-1)
            {
                b.x = i;
                b.y = j;
                for(k=0; k<4; k++)
                {
                    c = add_coords(b, add[k]);
                    if( !isInGrid(c) || matrice[c.x][c.y]==-1 )
                    {
                        tab[l++] = b;
                    }
                }
            }
        }
    }
    
    b.x = -99;
    b.y = -99;
    tab[l] = b;

    return (Coord *)tab;
}

Coord * get_border(int cha_id, abilityId Id, Coord coorTab[])
{
    Entity * e = e_from_id(cha_id);
    int matrice[_X_SIZE_][_Y_SIZE_];
    
    if(verbose)printf("Selected Entity : %s\n", e->cha_name);
    if(verbose)printf("Selected Entity pos : %d, %d\n", e->coords.x, e->coords.y);

    if(Id == Mvt)
    {
        setMovementZone(fill_tiles(e->coords, matrice, e->stat_mods[mv]), coorTab);
    }
    else
    {
        setActionZone(e->coords, get_range(e, Id), coorTab);
    }

    return coorTab;
}

bool Cast_check(action a, Coord coorTab[])
{
    Ability ab;
    int var;
    Entity * e = e_from_id(a.char_id);
    char log[STR_LONG];

    if(e->status_effect[Freezing])
    {
        sprintf(log, "%s is frozen and cannot do anything", e->cha_name);
        addLog(log);
        return FALSE;
    }

    if(a.act != Mvt)
    {

        ab = e->cha_class->cla_abilities[a.act%NUM_AB];
    }
    else
    {
        if(e->status_effect[Cripple])
        {
            sprintf(log, "%s is crippled and cannot move", e->cha_name);
            addLog(log);
            return FALSE;
        }

    }

    if((var = e->status_effect[Provoked]))
    {
        Entity * t = getEntity(a.c);
        if(t->cha_id != var)
        {
            Entity * g = e_from_id(var);
            sprintf(log, "%s is provoked by %s and can't do anything other than attack him", e->cha_name, g->cha_name);
            addLog(log);
            return FALSE;
        }
    }

    if(tile_type(a.c, ab.target, e))
    {
        if(isInRange(coorTab, a.c))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*ALGORITHM
    WHEN ABILITY SELECTED CALL GET BORDER
    IF MVT SHOW LOUIS BORDER
    IF AB SHOW LUCIEN BORDER
    COLLECT BORDER
    ONCE COORD SELECTED CALL CAST CHECK 
*/