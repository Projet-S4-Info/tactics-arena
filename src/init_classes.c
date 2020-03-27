#include <stdlib.h>
#include "abilities.h"

Coord * one_c;
Damage * one_a;

err_t init_repetitives()
{
    Coord * one_c = malloc(sizeof(Coord));
    if(one_c==NULL) return POINTER_NULL;
    Damage * one_a = malloc(sizeof(Damage));
    if(one_a==NULL) return POINTER_NULL;

    Coord ctemp = {0,0};
    Damage dtmp1 = {1,atk};

    *one_c = ctemp;
    *one_a = dtmp1;

    return OK;
}

err_t init_berserker(Class * c)
{
    Damage * FD = malloc(sizeof(Damage));
    if(FD==NULL) return POINTER_NULL;

    Damage dtmp = {0.8,atk};

    *FD = dtmp;

    Coord * ult=malloc(sizeof(Coord)*31);
    if(ult==NULL) return POINTER_NULL;
    int i=0,x=-2,y=-3;
    while(x<=2)
    {
        if(!(abs(x)==2&&abs(y)==3))
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

    Ability abtemp1 = {Slash,1,0,1,one_a,1,one_c,0,NULL,NULL,{"Slash", "Slash at an ennemy."},"../inc/sprites/berserker/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Killing_Blow,2,1,1,one_a,1,one_c,0,NULL,Killing_Blow_fn,{"Killing Blow","Slash at an ennemy, killing him increases attack permanently and gives one action point back."},"../inc/sprites/berserker/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Fury,2,2,0,NULL,1,one_c,0,NULL,Fury_fn,{"Fury","Remove all debuffs and permanently increase attack by the number of turns removed."},"../inc/sprites/berserker/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Frenzied_Dash,3,4,9,FD,31,ult,0,NULL,NULL,{"Frenzied Dash","Jump to a tile, dealing aoe damage when landing."},"../inc/sprites/berserker/sprite_sheet/A4/"};
    *(ab+3) = abtemp4;
    

    Class temp = 
    {
        Berserker,
        "Berserker",
        {20,8,10,10,0,10,10},
        {"Bloodlust","Killing two ennemies in the same turn refills action points and doubles mp and atk stats for one turn."},
        ab,
        "../inc/sprites/berserker/sprite_sheet/"
    };

    *c = temp;

    return OK;
}

err_t init_ranger(Class * c)
{
    Modifier * m = malloc(sizeof(Modifier)*3);
    if(m==NULL) return POINTER_NULL;
    Modifier t[3] = {{{7,vis,3},1,ALLIES},{{0,Piercing,3},1,ALLIES},{{-8,mv,3},1,ALLIES}};
    *m=t[0];
    *(m+1)=t[1];
    *(m+2)=t[2];

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Bolt,1,0,6,one_a,1,one_c,0,NULL,NULL,{"Bolt","Attack an ennemy from range."},"../inc/sprites/ranger/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Focus,2,4,0,NULL,1,one_c,0,NULL,Focus_fn,{"Focus","Increase by two turns all buffs and decrease by one all debuffs."},"../inc/sprites/ranger/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Trap,2,2,4,NULL,1,one_c,0,NULL,Trap_fn,{"Trap","Put a hidden trap that will cripple an ennemy that walks over it."},"../inc/sprites/ranger/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Deadeye,3,5,0,NULL,1,one_c,3,m,NULL,{"Deadeye","Increases range and makes bolt piercing but reduces mv for three turns."},"../inc/sprites/ranger/sprite_sheet/A4/"};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Ranger,
        "Ranger",
        {20,12,10,12,0,8,8},
        {"Sentinel","The first ennemy that moves to a spot inside a ranger's range every turn, will get bolted."},
        ab,
        "../inc/sprites/ranger/sprite_sheet/"
    };
    
    *c = temp;

    return OK;
}

err_t init_goliath(Class * c)
{

    Modifier * m = malloc(sizeof(Modifier)*2);
    if(m==NULL) return POINTER_NULL;
    Modifier t[2]= {{{-5,res_physic,1},1,FOES},{{-5,res_magic,1},1,FOES}};
    *m = t[0];
    *(m+1) = t[1];

    Modifier * n = malloc(sizeof(Modifier));
    if(n==NULL) return POINTER_NULL;
    Modifier f= {{0,Guarding,1},1,ALLIES};
    *n = f;

    Modifier * d = malloc(sizeof(Modifier));
    if(d==NULL) return POINTER_NULL;
    Modifier l= {{0,Detained,3},1,FOES};
    *d = l;

    Modifier * u = malloc(sizeof(Modifier));
    if(d==NULL) return POINTER_NULL;
    Modifier ut= {{0,Provoked,1},1,FOES};
    *u= ut;

    Coord * ult=malloc(sizeof(Coord)*31);
    if(ult==NULL) return POINTER_NULL;
    int i=0,x=-3,y=-4;
    while(x<=3)
    {
        if(!(( abs(x)==3 && (abs(y)==4 || abs(y)==3) ) || (abs(x)==2 && abs(y)==4) ))
        {
            ult[i].x=x;
            ult[i].y=y;
            i++;
        }
        if((y=4))
        {
            y=-4;
            x++;
        }
        else y++;
    }

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Bash,1,0,1,one_a,1,one_c,2,m,NULL,{"Bash","Damage and reduce the ennemy's resistances for a turn."},"../inc/sprites/goliath/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Shields_Up,2,1,0,NULL,1,one_c,1,n,NULL,{"Shields Up","Increase passive block chance to 70% for one turn."},"../inc/sprites/goliath/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Detain,2,4,2,NULL,1,one_c,1,d,NULL,{"Detain","Capture an Ennemy for three turns."},"../inc/sprites/goliath/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Banner,3,5,0,NULL,51,ult,1,u,Banner_fn,{"Banner","Provoke all ennemies in a zone for one turn and reset all allies' cooldowns."},"../inc/sprites/goliath/sprite_sheet/A4/"};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Goliath,
        "Goliath",
        {20,6,10,8,0,14,14},
        {"Guardian","Has a 30% chance to block incoming damage."},
        ab,
        "../inc/sprites/goliath/sprite_sheet/"
    };
    
    *c = temp;

    return OK;
}