#include <string.h>
#include <stdlib.h>
#include "init.h"
#include "grid.h"
#include "game_window.h"
#include "display.h"

err_t get_team(Entity *e, Entity **all, bool same)
{
    if(same)
    {
        if(e->cha_id<0)
        {
            *all = Foes;
        }
        else
        {
            *all = Allies;
        }
    }
    else
    {
        if(e->cha_id<0)
        {
            *all = Allies;
        }
        else
        {
            *all = Foes;
        }
    }

    return OK;
}

winId game_over()
{
    winId all_dead = LOSE;
    int i;

    for(i=0; i<NUM_CLASS && all_dead; i++)
    {
        all_dead = Allies[i].active ? ONGOING : LOSE;
    }

    if(!all_dead)
    {
        all_dead = WIN;
        for(i=0; i<NUM_CLASS && all_dead; i++)
        {
            all_dead = Foes[i].active ? ONGOING : WIN;
        }
    }

    return all_dead;
}

int get_range(int vision, int range_mod)
{
    int range = range_mod * (vision/10);

    if(range<=0)
    {
        range = 1;
    }

    return range;
}

char * get_name(Entity * e, abilityId ab_id)
{
    if(ab_id!=Mvt)
    {
        return e->cha_class->cla_abilities[ab_id%NUM_AB].eng.name;
    }
    else
    {
        return Move_ab.eng.name;
    }
    
}

char * get_desc(Entity * e, abilityId ab_id)
{
    if(ab_id!=Mvt)
    {
        return e->cha_class->cla_abilities[ab_id%NUM_AB].eng.desc;
    }
    else
    {
        return Move_ab.eng.desc;
    }
}

bool able_ability(Entity *e, abilityId ab_id)
{
    return e->act_points >= e->cha_class->cla_abilities[ab_id%NUM_AB].ab_cost;
}

bool same_team(Entity *a, Entity *b)
{
    if(a->cha_id<0&&b->cha_id<0)
    {
        return TRUE;
    }
    else if(a->cha_id>0&&b->cha_id>0)
    {
        return TRUE;
    }else
    {
        return FALSE;
    }
}

err_t reset_cooldowns(Entity * e)
{
    int i;
    for(i=0; i<NUM_AB; i++)
    {
        e->ab_cooldown[i] = 0;
    }

    return OK;
}

bool closer_coords(Coord a, Coord b)
{
    return sqrt(a.x*a.x+a.y+a.y) <= sqrt(b.x*b.x+b.y*b.y);
}

Coord compare_coords(Coord a, Coord b)
{
    Coord c;

    c.x = b.x - a.x;
    c.y = b.y - a.y;

    return c;
}

Coord add_coords(Coord a, Coord b)
{
    Coord c;

    c.x = a.x + b.x;
    c.y = a.y + b.y;

    if(verbose)printf("Coords a(%d,%d) and b(%d,%d) added to give c(%d,%d)\n", a.x, a.y, b.x, b.y, c.x, c.y);

    return c;
}

err_t remove_mod(Status * stat, Entity * e)
{
    char log[STR_LONG];

    if(stat->value==0)
    {
        if(verbose)printf("Modifier to remove is a status effect!\n");
        e->status_effect[stat->stat] = 0;
        sprintf(log, "%s is no longer %s", e->cha_name, statusName[stat->stat]);
    }
    else
    {
        if(verbose)printf("Modifier to remove is a stat change of a value of %d\n", stat->value);
        if(verbose)printf("Stat before the change : %d\n", e->stat_mods[stat->stat]);
        e->stat_mods[stat->stat] += stat->value *-1;
        if(verbose)printf("Stat after the change : %d\n", e->stat_mods[stat->stat]);
        sprintf(log, "%s is no longer %s", e->cha_name, statName[stat->stat]);
    }

    addLog(log);

    return OK;
}

Status * renew_mod(Entity * e, statusId status)
{
    Status * v;

    start_list(stReceived);
    v = list_search(stReceived,e,-status);
    list_remove(stReceived);

    if(v==NULL)
    {
        start_list(stSent);
        v = list_search(stSent,e,-status);
        list_remove(stSent);
    }

    return v;
}

err_t new_death(Entity * e)
{
    reset_cooldowns(e);

    e->active = Dead;
    e->stat_mods[pv]=20;

    Status * v;

    start_list(stSent);
    while((v = list_search(stSent,e,-1))!=NULL)
    {
        remove_mod(v, e);
        list_remove(stSent);
        list_next(stSent);
    }

    start_list(stReceived);
    while((v = list_search(stReceived,e,-1))!=NULL)
    {
        remove_mod(v, e);
        list_remove(stReceived);
        list_next(stReceived);
    }

    char log[STR_LONG];
    sprintf(log, "%s has died", e->cha_name);
    addLog(log);

    //PLAY DEATH ANIMATION


    if(verbose)printf("%s has been killed!\n", e->cha_name);

    return OK;
}

bool death_check(Entity * e)
{
    if(e->stat_mods[pv]<=0)
    {
        if(verbose)printf("%s was killed!\n", e->cha_name);
        new_death(e);
        return TRUE;
    }
    else
        return FALSE;
}

bool apply_damage(Damage * d, Entity * caster, Entity * target)
{

    char log[STR_LONG];


    if(target->cha_id==Goliath)
    {
        int block = target->status_effect[Guarding] == 1 ? 70 : 30;

        if(verbose)printf("Block chance : %d\n", block);

        if(block>=(rand()%100+1))
        {
            if(verbose)printf("Block Successful!\n");
            sprintf(log, "%s blocked the incoming damage", target->cha_name);
            addLog(log);
            //PLAY ANIMATION
            return FALSE;
        }
        else
        {
            if(verbose)printf("Block Failed!\n");
        }
        
    }

    float crippled = target->status_effect[Cripple] == 1 ? 1.75 : 1;

    if(crippled==1.75)
        if(verbose)printf("%s is crippled!\n", target->cha_name);

    if(verbose)printf("%s's health before the attack : %d\n", target->cha_name, target->stat_mods[pv]);

    int d_value;

    if(caster->status_effect[Piercing])
    {
        if(verbose)printf("%s has piercing!\n", caster->cha_name);
        target->stat_mods[pv] -= d_value = (caster->stat_mods[d->type] * d->multiplier * crippled)+0.4;
        sprintf(log, "%s pierced %s's defence and dealt %d damage", caster->cha_name, target->cha_name, d_value);
    }
    else
    {
        target->stat_mods[pv] -= d_value = (caster->stat_mods[d->type]/(1+(target->stat_mods[d->type+2]/15)) * d->multiplier * crippled)+0.4;
        sprintf(log, "%s dealt %d damage to %s", caster->cha_name, d_value, target->cha_name);
    }

    if(verbose)printf("%s's health after the attack : %d\n", target->cha_name, target->stat_mods[pv]);

    addLog(log);

    return death_check(target);

}

err_t apply_status(Status s, Entity *target, StateList *list, int caster_id)
{

    char log[STR_LONG];

    if(verbose)printf("Modifier is a status effect!\n");

    if(target->status_effect[s.stat])
    {
        renew_mod(target, s.stat);
        sprintf(log, "%s is still %s", target->cha_name, statusName[s.stat]);
    }

    else if(s.stat==Provoked)
    {
        target->status_effect[s.stat] = caster_id;

        Entity * caster = caster_id<0 ? &Foes[abs(caster_id)-1] : &Allies[abs(caster_id)-1];

        sprintf(log, "%s is provoked by %s", target->cha_name, caster->cha_name);
    }

    else if(s.stat==Burning && target->status_effect[Freezing])
    {
        remove_mod(renew_mod(target, Freezing),target);
        if(verbose)printf("Attempting to burn %s has thawed him out!\n", target->cha_name);
        sprintf(log, "Attempting to burn %s has thawed him out", target->cha_name);
        addLog(log);
        return OK;
    }

    else if(s.stat==Freezing && target->status_effect[Burning])
    {
        if(verbose)printf("Target is burning, cannot be frozen!\n");
        sprintf(log, "%s is burning and cannot be frozen", target->cha_name);
        addLog(log);
        return OK;
    }

    else
    {
        target->status_effect[s.stat] = 1;
        sprintf(log, "%s is %s", target->cha_name, statusName[s.stat]);
    }

    if(verbose)printf("Status effect : %d", target->status_effect[s.stat]);

    if(s.duration!=0)
    {
        if(verbose)printf("The mod will last %d turns!\n", s.duration);
        char log_t[STR_SHORT];
        sprintf(log_t, " for %d turns", s.duration);
        strcat(log, log_t);
        addLog(log);
        return list_add(list, s, target);
    }
    else
    {
        if(verbose) printf("The mod is permanent!\n");
        strcat(log, " permanently");
        addLog(log);
        return OK;
    }

}

err_t apply_stat_change(Status s, Entity * target, StateList * list)
{
    char log[STR_LONG];

    if(verbose)printf("Modifier is a stat change of %d\n", s.value);
    if(verbose)printf("Stat before the change : %d\n", target->stat_mods[s.stat]);

    target->stat_mods[s.stat] += s.value;

    if(target->stat_mods[s.stat]>20)
    {
        s.value -= target->stat_mods[s.stat] - 20;
        target->stat_mods[s.stat] = 20;
    }
    else if(target->stat_mods[s.stat]<0)
    {
        s.value -= target->stat_mods[s.stat];
        target->stat_mods[s.stat] = 0;
    }

    sprintf(log, "%s's %s was altered by %d", target->cha_name, statName[s.stat], s.value);
    
    if(verbose)printf("Stat after the change : %d\n", target->stat_mods[s.stat]);

    if(s.duration!=0)
    {
        if(verbose)printf("The mod will last %d turns!\n", s.duration);
        char log_t[STR_SHORT];
        sprintf(log_t, " for %d turns", s.duration);
        strcat(log, log_t);
        addLog(log);
        return list_add(list, s, target);
    }
    else
    {
        if(verbose) printf("The mod is permanent!\n");
        strcat(log, " permanently");
        addLog(log);
        return OK;
    }
}

err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id)
{
    if(m.chance*100>=(rand()%100+1))
    {
        if(verbose)printf("Modifier landed!\n");

        if(m.effect.value==0)
        {
            return apply_status(m.effect,target, list, caster_id);
        }
        else
        {
            return apply_stat_change(m.effect,target, list);
        }
    }
    else
    {
        if(verbose)printf("Modifier not landed!\n");
        return OK;
    }
}

int apply_to(Ability active_ab, Entity * active_ent, StateList * list, Coord starting_point)
{
    int death_count = 0;
    int i,j;
    Coord c;
    Entity * e;

        for(i=0; i<active_ab.nb_coords; i++)
        {
            c=add_coords(starting_point, *(active_ab.coord[i]));

            e=getEntity(c);
            
            if(e!=NULL)
            {
                if(verbose)printf("%s was found in the zone!\n", e->cha_name);

                if(!same_team(e,active_ent))
                {
                    if(verbose)printf("%s is an Ennemy!\n", e->cha_name);

                    if(active_ab.damage!=NULL)
                    {
                        if(apply_damage(*(active_ab.damage), active_ent, e))
                        {
                            death_count++;
                        }
                    }

                }
                else
                {
                    if(verbose)printf("%s is an Ally!\n", e->cha_name);
                }

                if(active_ab.mods!=NULL && e->active!=Dead)
                {
                    for(j=0; j<active_ab.nb_mods; j++)
                    {
                        if(!same_team(e,active_ent) && (*(active_ab.mods)+j)->t!=ALLIES)
                            apply_mod(*(active_ab.mods)[j],e, list, active_ent->cha_id);

                        else if((same_team(e,active_ent) && (*(active_ab.mods)+j)->t!=FOES))
                            apply_mod(*(active_ab.mods)[j],e, list, active_ent->cha_id);
                    }
                }

            }
            if(active_ab.fn_use==DURING)
            {
                death_count += active_ab.function(c,active_ent,list); 
            }
        }

    return death_count;
}

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


int isInRange(Coord coorTab[], Coord attack){
    
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
    if(touchLine == 1){
        return 1;
    }else{
        return 0;
    }
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