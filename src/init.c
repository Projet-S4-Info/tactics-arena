#include <stdlib.h>
#include "struct.h"

err_t init_berserker(Class * c)
{
    Coord * one_c = malloc(sizeof(Coord));
    if(one_c==NULL) return POINTER_NULL;
    Damage * one = malloc(sizeof(Damage));
    if(one==NULL) return POINTER_NULL;
    Damage * FD = malloc(sizeof(Damage));
    if(FD==NULL) return POINTER_NULL;
    Coord ctemp = {0,0};
    Damage dtmp1 = {1,atk};
    Damage dtmp2 = {0.8,atk};
    *one_c = ctemp;
    *one = dtmp1;
    *FD = dtmp2;

    Coord * ult=malloc(sizeof(Coord)*31);
    if(ult==NULL) return POINTER_NULL;
    int i,x,y;
    for(i=0,x=-2,y=-4;x<=2;)
    {
        if(!((x==-2&&y==-3)||(x==-2&&y==3)||(x==2&&y==-3)||(x==2&&y==3)))
        {
            ult[i].x=x;
            ult[i].y=y;
            i++;
        }
        if((y=3))
        {
            y=-3;
            x++;
        }
        else y++;
    }

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Slash,1,0,1,one,1,one_c,0,NULL,NULL,{"Slash", "Slash at an ennemy."},"../inc/sprites/berserker/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Killing_Blow,2,1,1,one,1,one_c,0,NULL,NULL,{"Killing Blow","Slash at an ennemy, killing him increases attack permanently and gives one action point back."},"../inc/sprites/berserker/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Fury,2,2,0,NULL,0,NULL,0,NULL,NULL,{"Fury","Remove all debuffs and permanently increase attack by the number of turns removed."},"../inc/sprites/berserker/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Frenzied_Dash,3,4,9,FD,31,ult,0,NULL,NULL,{"Frenzied Dash","Jump to a tile, dealing aoe damage when landing."},"../inc/sprites/berserker/sprite_sheet/A4/"};
    *(ab+3) = abtemp4;
    

    Class temp = {Berserker,"Berserker",{13,8,10,10,0,10,10},{"Bloodlust","Killing two ennemies in the same turn refills action points and doubles mp and atk stats for one turn."},ab,"../inc/sprites/berserker/sprite_sheet/"};

    *c = temp;

    return OK;
}

err_t init_game()
{
    return OK;
}