#include <stdlib.h>
#include "abilities.h"

Coord * one_c;
Damage * one_a;
Damage * one_m;

err_t init_repetitives()
{
    Coord * one_c = malloc(sizeof(Coord));
    if(one_c==NULL) return POINTER_NULL;
    Damage * one_a = malloc(sizeof(Damage));
    if(one_a==NULL) return POINTER_NULL;
    Damage * one_m = malloc(sizeof(Damage));
    if(one_m==NULL) return POINTER_NULL;

    Coord ctemp = {0,0};
    Damage dtmp1 = {1,atk};
    Damage dtmp2 = {1,magic};

    *one_c = ctemp;
    *one_a = dtmp1;
    *one_m = dtmp2;

    return OK;
}

err_t init_berserker(Class * c)
{
    Damage * FD = malloc(sizeof(Damage));
    if(FD==NULL) return POINTER_NULL;

    Damage dtmp = {0.8,atk};

    *FD = dtmp;

    Coord * ult=malloc(sizeof(Coord)*51);
    if(ult==NULL) return POINTER_NULL;
    int i=0,x=-3,y=-4;
    while(x<=3)
    {
        if(!(( abs(x)==3 && abs(y)>=3) || (abs(x)==2 && abs(y)==4) ))
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

    if(i!=51)
        return INIT_COORD_ERR;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Slash,1,0,1,one_a,1,one_c,0,NULL,FALSE,NULL,{"Slash", "Slash at an ennemy."},"../inc/sprites/berserker/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Killing_Blow,2,1,1,one_a,1,one_c,0,NULL,TRUE,Killing_Blow_fn,{"Killing Blow","Slash at an ennemy, killing him increases attack permanently and gives one action point back."},"../inc/sprites/berserker/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Fury,2,2,0,NULL,1,one_c,0,NULL,TRUE,Fury_fn,{"Fury","Remove all debuffs and permanently increase attack by the number of turns removed."},"../inc/sprites/berserker/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Frenzied_Dash,3,4,9,FD,i,ult,0,NULL,FALSE,NULL,{"Frenzied Dash","Jump to a tile, dealing aoe damage when landing."},"../inc/sprites/berserker/sprite_sheet/A4/"};
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

    Ability abtemp1 = {Bolt,1,0,6,one_a,1,one_c,0,NULL,FALSE,NULL,{"Bolt","Attack an ennemy from range."},"../inc/sprites/ranger/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Focus,2,4,0,NULL,1,one_c,0,NULL,TRUE,Focus_fn,{"Focus","Increase by two turns all buffs and decrease by one all debuffs."},"../inc/sprites/ranger/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Trap,2,2,4,NULL,1,one_c,0,NULL,TRUE,Trap_fn,{"Trap","Put a hidden trap that will cripple an ennemy that walks over it."},"../inc/sprites/ranger/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Deadeye,3,5,0,NULL,1,one_c,3,m,FALSE,NULL,{"Deadeye","Increases range and makes bolt piercing but reduces mv for three turns."},"../inc/sprites/ranger/sprite_sheet/A4/"};
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

    Coord * ult=malloc(sizeof(Coord)*103);
    if(ult==NULL) return POINTER_NULL;
    int i=0,x=-5,y=-6;
    while(x<=5)
    {
        if(!(( abs(x)==5 && abs(y)>=3 ) || (abs(y)==6 && abs(x)>=2) || (abs(x)==4 && abs(y)>=4) || (abs(x)==3 && abs(y)==5)))
        {
            ult[i].x=x;
            ult[i].y=y;
            i++;
        }
        if((y=6))
        {
            y=-6;
            x++;
        }
        else y++;
    }

    if(i!=103)
        return INIT_COORD_ERR;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Bash,1,0,1,one_a,1,one_c,2,m,FALSE,NULL,{"Bash","Damage and reduce the ennemy's resistances for a turn."},"../inc/sprites/goliath/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Shields_Up,2,1,0,NULL,1,one_c,1,n,FALSE,NULL,{"Shields Up","Increase passive block chance to 70% for one turn."},"../inc/sprites/goliath/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Detain,2,4,2,NULL,1,one_c,1,d,FALSE,NULL,{"Detain","Capture an Ennemy for three turns."},"../inc/sprites/goliath/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Banner,3,5,0,NULL,i,ult,1,u,FALSE,Banner_fn,{"Banner","Provoke all ennemies in a zone for one turn and reset all allies' cooldowns."},"../inc/sprites/goliath/sprite_sheet/A4/"};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Goliath,
        "Goliath",
        {20,6,10,8,0,14,12},
        {"Guardian","Has a 30% chance to block incoming damage."},
        ab,
        "../inc/sprites/goliath/sprite_sheet/"
    };
    
    *c = temp;

    return OK;
}

err_t init_mage(Class * c, Ability movesets[3][NUM_AB])
{
    Damage * d = malloc(sizeof(Damage));
    if(d==NULL) return POINTER_NULL;
    Damage l = {1.3,magic};
    *d = l;

    Damage * u = malloc(sizeof(Damage));
    if(u==NULL) return POINTER_NULL;
    Damage r = {1.5,magic};
    *u = r;

    Damage * red = malloc(sizeof(Damage));
    if(red==NULL) return POINTER_NULL;
    Damage k = {0.8,magic};
    *red = k;

    Modifier * fire1= malloc(sizeof(Modifier));
    if(fire1==NULL) return POINTER_NULL;
    Modifier f1 = {{0,Burning,3},0.6,BOTH};
    *fire1 = f1;

    Modifier * fire2= malloc(sizeof(Modifier));
    if(fire2==NULL) return POINTER_NULL;
    Modifier f2 = {{0,Burning,3},1,BOTH};
    *fire2 = f2;

    Modifier * ice1= malloc(sizeof(Modifier));
    if(ice1==NULL) return POINTER_NULL;
    Modifier ie1 = {{0,Freezing,3},0.3,FOES};
    *ice1 = ie1;

    Modifier * ice2= malloc(sizeof(Modifier));
    if(ice2==NULL) return POINTER_NULL;
    Modifier ie2 = {{0,Freezing,3},1,FOES};
    *ice2 = ie2;

    Modifier * ice3= malloc(sizeof(Modifier)*2);
    if(ice3==NULL) return POINTER_NULL;
    Modifier ie3[2] = {ie1,{{-8,vis,3},1,FOES}};
    *ice3 = ie3[0];
    *(ice3+1) = ie3[1];

    Modifier * armor= malloc(sizeof(Modifier)*2);
    if(armor==NULL) return POINTER_NULL;
    Modifier arm[2] = {{{6,res_physic,2},1,ALLIES},{{6,res_magic,2},1,ALLIES}};
    *armor = arm[0];
    *(armor+1) = arm[1];

    Modifier * thera= malloc(sizeof(Modifier)*2);
    if(thera==NULL) return POINTER_NULL;
    Modifier py[2] = {{{6,atk,2},1,ALLIES},{{6,magic,2},1,ALLIES}};
    *thera = py[0];
    *(thera+1) = py[1];

    Coord * fla=malloc(sizeof(Coord)*103);
    if(fla==NULL) return POINTER_NULL;
    int i=0,x=-5,y=-6;
    while(x<=5)
    {
        if(!(( abs(x)==5 && abs(y)>=3 ) || (abs(y)==6 && abs(x)>=2) || (abs(x)==4 && abs(y)>=4) || (abs(x)==3 && abs(y)==5)))
        {
            fla[i].x=x;
            fla[i].y=y;
            i++;
        }
        if((y=6))
        {
            y=-6;
            x++;
        }
        else y++;
    }

    if(i!=103)
        return INIT_COORD_ERR;

    Coord * ult=malloc(sizeof(Coord)*71);
    if(ult==NULL) return POINTER_NULL;
    int i1=0;
    x=-4,y=-5;
    while(x<=5)
    {
        if(!(( abs(x)==4 && abs(y)>=3 ) || (abs(y)==5 && abs(x)>=2)  || (abs(x)==3 && abs(y)==4)))
        {
            ult[i1].x=x;
            ult[i1].y=y;
            i1++;
        }
        if((y=5))
        {
            y=-5;
            x++;
        }
        else y++;
    }

    if(i1!=71)
        return INIT_COORD_ERR;

    Coord * vs=malloc(sizeof(Coord)*51);
    if(vs==NULL) return POINTER_NULL;
    int i2=0;
    x=-3,y=-4;
    while(x<=3)
    {
        if(!(( abs(x)==3 && abs(y)>=3) || (abs(x)==2 && abs(y)==4) ))
        {
            vs[i2].x=x;
            vs[i2].y=y;
            i2++;
        }
        if((y=4))
        {
            y=-4;
            x++;
        }
        else y++;
    }

    if(i2!=51)
        return INIT_COORD_ERR;


    Ability abtemp1 = {Fireball,1,0,8,one_m,1,one_c,1,fire1,FALSE,mage_switch,{"Fireball","Deals damage and has a chance to set target ablaze."},"../inc/sprites/mage/sprite_sheet/A1/fire"};
    movesets[0][0] = abtemp1;
    Ability abtemp2 = {FlameCharge,2,3,10,one_m,1,one_c,1,fire2,TRUE,FlameCharge_fn,{"FlameCharge","Engulf yourself in flames and dash, dealing damage and burning entities in your path."},"../inc/sprites/mage/sprite_sheet/A2/fire"};
    movesets[0][1] = abtemp2;
    Ability abtemp3 = {Flare,2,3,5,NULL,i,fla,0,NULL,TRUE,Flare_fn,{"Flare","Increases all allies' vision and their mp for two turns, will spot traps in the chosen area."},"../inc/sprites/mage/sprite_sheet/A3/fire"};
    movesets[0][2] = abtemp3;
    Ability abtemp4 = {Eruption,3,5,12,u,i1,ult,1,fire2,FALSE,NULL,{"Eruption","Deal massive damage in a zone and burn all entities."},"../inc/sprites/mage/sprite_sheet/A4/fire"};
    movesets[0][3] = abtemp4;

    Ability abtemp11 = {Icy_Winds,1,0,8,one_m,1,one_c,1,ice1,FALSE,mage_switch,{"Icy Winds","Deals damage and has a chance to freeze target."},"../inc/sprites/mage/sprite_sheet/A1/ice"};
    movesets[1][0] = abtemp11;
    Ability abtemp12 = {Freeze,2,3,8,red,1,one_c,1,ice2,FALSE,NULL,{"Freeze","Deal damage to and freeze an ennemy. "},"../inc/sprites/mage/sprite_sheet/A2/ice"};
    movesets[1][1] = abtemp12;
    Ability abtemp13 = {Frozen_Armor,2,3,6,NULL,1,one_c,2,armor,FALSE,NULL,{"Frozen Armor","Increase ally physical and magic resistances."},"../inc/sprites/mage/sprite_sheet/A3/ice"};
    movesets[1][2] = abtemp13;
    Ability abtemp14 = {Blizzard,3,5,12,d,i1,ult,2,ice3,TRUE,Blizzard_fn,{"Blizzard","Deal damage in a zone and highly reduce ennemy vision for a turn, has a chance to freeze ennemies. Water tiles in the are will also freeze."},"../inc/sprites/mage/sprite_sheet/A4/ice"};
    movesets[1][3] = abtemp14;

    Ability abtemp21 = {Shock,1,0,8,d,1,one_c,0,NULL,FALSE,mage_switch,{"Shock","Zap an ennemy."},"../inc/sprites/mage/sprite_sheet/A1/lightning"};
    movesets[2][0] = abtemp21;
    Ability abtemp22 = {Volt_Switch,2,3,10,one_m,i2,vs,0,NULL,TRUE,Volt_Switch_fn,{"Volt Switch","Switch spots with an ally and deal electric damage around caster."},"../inc/sprites/mage/sprite_sheet/A2/lightning"};
    movesets[2][1] = abtemp22;
    Ability abtemp23 = {Shock_Therapy,2,3,6,NULL,1,one_c,2,thera,FALSE,NULL,{"Shock Therapy","Increase ally atk and magic."},"../inc/sprites/mage/sprite_sheet/A3/lightning"};
    movesets[2][2] = abtemp23;
    Ability abtemp24 = {Lightning_Chain,3,5,8,d,1,one_c,0,NULL,FALSE,Lightning_Chain_fn,{"Lightning Chain","Zap an ennemy, the bolt will bounce to another close ennemy until either there are no ennemies close enough or it has bounced 3 times."},"../inc/sprites/mage/sprite_sheet/A4/lightning"};
    movesets[2][3] = abtemp24;

    Class temp = 
    {
        Mage,
        "Mage",
        {20,7,10,0,14,8,12},
        {"Elementalist","After every use of a basic attack, change element (Fire, Ice, Electricity)."},
        NULL,
        "../inc/sprites/mage/sprite_sheet/"
    };

    *c = temp;

    return OK;
}

err_t init_valkyrie(Class * c)
{
    Modifier * para= malloc(sizeof(Modifier));
    if(para==NULL) return POINTER_NULL;
    Modifier p = {{0,Paralyzed,2},0.4,FOES};
    *para = p;

    Coord * rav=malloc(sizeof(Coord)*51);
    if(rav==NULL) return POINTER_NULL;
    int i=0,x=-3,y=-4;
    while(x<=3)
    {
        if(!(( abs(x)==3 && abs(y)>=3) || (abs(x)==2 && abs(y)==4) ))
        {
            rav[i].x=x;
            rav[i].y=y;
            i++;
        }
        if((y=4))
        {
            y=-4;
            x++;
        }
        else y++;
    }
    if(i!=51)
        return INIT_COORD_ERR;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Thrust,1,0,1,one_a,1,one_c,1,para,FALSE,Thrust_fn,{"Thrust","Thrust your spear forward."},"../inc/sprites/valkyrie/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Odins_Eyes,2,2,8,one_m,i,rav,1,para,FALSE,NULL,{"Odin's Eyes","Call forth a swarm of crows to deal magic damage to an area."},"../inc/sprites/valkyrie/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Life_Transfer,2,3,8,NULL,1,one_c,0,NULL,TRUE,Life_Transfer_fn,{"Life Transfer","Select an Ennemy and an Ally, heal the ally back to full and slow the ennemy by the amount healed."},"../inc/sprites/valkyrie/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Gates_of_Valhalla,3,5,0,NULL,0,NULL,0,NULL,TRUE,Gates_of_Valhalla_fn,{"Gates of Valhalla","Resurrect all dead allies for a turn."},"../inc/sprites/valkyrie/sprite_sheet/A4/"};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Valkyrie,
        "Valkyrie",
        {20,10,10,10,10,10,10},
        {"Precision","Damaging an ennemy has a chance to paralyze them."},
        ab,
        "../inc/sprites/valkyrie/sprite_sheet/"
    };
    
    *c = temp;

    return OK;
}

err_t init_angel(Class * c)
{
    Modifier * heal= malloc(sizeof(Modifier));
    if(heal==NULL) return POINTER_NULL;
    Modifier p = {{6,pv,0},1,ALLIES};
    *heal = p;

    Modifier * bless= malloc(sizeof(Modifier));
    if(bless==NULL) return POINTER_NULL;
    Modifier pb = {{0,Blessed,1},1,ALLIES};
    *bless = pb;

    Coord * sto=malloc(sizeof(Coord)*51);
    if(sto==NULL) return POINTER_NULL;
    int i=0,x=-3,y=-4;
    while(x<=3)
    {
        if(!(( abs(x)==3 && abs(y)>=3) || (abs(x)==2 && abs(y)==4) ))
        {
            sto[i].x=x;
            sto[i].y=y;
            i++;
        }
        if((y=4))
        {
            y=-4;
            x++;
        }
        else y++;
    }
    if(i!=51)
        return INIT_COORD_ERR;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Condemn,1,0,6,one_m,1,one_c,0,NULL,FALSE,NULL,{"Condemn","Deal magic damage to an ennemy."},"../inc/sprites/angel/sprite_sheet/A1/"};
    *ab = abtemp1;
    Ability abtemp2 = {Holy_Storm,2,3,8,one_m,i,sto,1,heal,FALSE,NULL,{"Holy Storm","Deal magic damage to ennemies in an area and heal all allies in the area."},"../inc/sprites/angel/sprite_sheet/A2/"};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Last_Sacrfice,2,5,0,NULL,0,NULL,0,NULL,TRUE,Last_Sacrfice_fn,{"Last Sacrfice","Kill self to resurrect an Ally."},"../inc/sprites/angel/sprite_sheet/A3/"};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Gods_Blessing,3,5,8,NULL,1,one_c,1,bless,FALSE,NULL,{"God's Blessing","Double an Ally's action points and apply no cooldowns for a turn."},"../inc/sprites/angel/sprite_sheet/A4/"};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Angel,
        "Angel",
        {20,8,10,0,10,12,12},
        {"Aura","Heal all nearby allies at start of your turn."},
        ab,
        "../inc/sprites/angel/sprite_sheet/"
    };
    
    *c = temp;

    return OK;
}