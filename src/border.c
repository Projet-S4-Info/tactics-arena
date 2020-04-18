#include "deplacement.h"
#include "struct.h"
#include "grid.h"
#include "gameplay.h"
#include "display.h"
#include "print.h"

bool isInRange(Coord coorTab[], Coord target)
{
    int linesCrossed = 0;

    int i;

    Coord c = {target.x, 0};

    while(c.y<target.y)
    {
        for(i=0; coorTab[i].x!=-99 ; i++)
        {
            if(same_coord(c,coorTab[i]))
            {
                linesCrossed++;
                break;
            }
        }
        c.y++;
    }

    for(i=0; coorTab[i].x!=-99 ; i++)
    {
        if(same_coord(c,coorTab[i]))
        {
            return TRUE;
        }
    }
    c.y++;

    if(linesCrossed>0)
    {
        while(c.y<_Y_SIZE_)
        {
            for(i=0; coorTab[i].x!=-99 ; i++)
            {
                if(same_coord(c,coorTab[i]))
                {
                    return TRUE;
                }
            }
            c.y++;
        }
    }
    
    return FALSE;
}

Coord normalize(Coord c)
{
    if(c.x>=_X_SIZE_)
    {
        c.x = _X_SIZE_-1;
    }
    else if(c.x<0)
    {
        c.x=0;
    }

    if(c.y>=_Y_SIZE_)
    {
        c.y = _Y_SIZE_-1;
    }
    else if(c.y<0)
    {
        c.y = 0;
    }

    return c;
}

Coord * setActionBorder(Coord start, int range, Coord tab[])
{
    int j;
    if(range>0)
    {
        Coord max[4] = {{0,range},{-range,0},{0,-range},{range,0}};
        Coord add[4] = {{-1,1},{-1,-1},{1,-1},{1,1}};

        int i;
        bool matrice[_X_SIZE_][_Y_SIZE_];

        for(i=0; i<_X_SIZE_; i++)
        {
            for(j=0; j<_Y_SIZE_; j++)
            {
                matrice[i][j] = TRUE;
            }
        }

        Coord c, norm, goal;
        Coord active = add_coords(max[3], start);

        j=0;

        for(i=0;i<4;i++)
        {
            goal = add_coords(start,max[i]);

            if(verbose)print_Coord(&goal, "Goal : ");

            while(!same_coord(c = add_coords(active,add[i]), goal))
            {
                norm = normalize(c);

                if(verbose)
                {
                    print_Coord(&active, "Active : ");
                    print_Coord(&c, "c : ");
                    print_Coord(&norm, "Norm : ");
                }
                
                if(matrice[norm.x][norm.y])
                {
                    if(verbose)printf("Adding\n");
                    tab[j++] = norm;
                    matrice[norm.x][norm.y] = FALSE;
                }
                else
                {
                    if(verbose)printf("Not Adding\n");
                }
                active = c;
            }


            norm = normalize(c);
            if(verbose)print_Coord(&norm, "Norm : ");
            if(matrice[norm.x][norm.y])
            {
                if(verbose)printf("Adding\n");
                tab[j++] = norm;
                matrice[norm.x][norm.y] = FALSE;
            }
            else
            {
                if(verbose)printf("Not Adding\n");
            }
            active = c;
        }

    }
    else
    {
        j=0;
        tab[j++] = start;
    }

    Coord r = {-99,-99};
    tab[j] = r;

    if(verbose)printf("Border Size : %d\n", j);

    return (Coord *)tab;
}

Coord * setActionZone(int range, Coord Border[], Coord Zone[])
{
    int k = 0;

    if(range>0)
    {
        int i,j;
        Coord c;

        for(i=0; i<_X_SIZE_; i++)
        {
            for(j=0; j<_Y_SIZE_; j++)
            {
                c.x = i;
                c.y = j;
                if(isInRange(Border, c))
                {
                    Zone[k++] = c;
                    printf("%d,%d is in range\n", c.x, c.y);
                }
                else
                {
                    printf("%d,%d is not in range\n", c.x, c.y);
                }
            }
        }
    }
    else
    {
        Zone[k] = Border[k];
        k++;
    }

    Coord d = {-99,-99};
    Zone[k] = d;

    if(verbose)printf("Zone Size : %d\n", k);

    return (Coord *)Zone;
}

Coord * setMovementBorder(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[])
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

    if(verbose)printf("Border Size : %d\n", l);

    return (Coord *)tab;
}

Coord * setMovementZone(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[])
{
    int i,j,k = 0;
    Coord c;

    for(i=0; i<_X_SIZE_; i++)
    {
        for(j=0; j<_Y_SIZE_; j++)
        {
            if(matrice[i][j]>0)
            {
                c.x = i;
                c.y = j;
                tab[k++] = c;
            }
        }
    }
    c.x = -99;
    c.y = -99;
    tab[k] = c;

    if(verbose)printf("Zone Size : %d\n", k);

    return (Coord *)tab;
}

Coord * get_border(int cha_id, abilityId Id, Coord coorTab[], Coord zone[])
{
    Entity * e = e_from_id(cha_id);
    int matrice[_X_SIZE_][_Y_SIZE_];

    if(verbose)printf("Selected Entity : %s\n", e->cha_name);
    if(verbose)printf("Selected Entity pos : %d, %d\n", e->coords.x, e->coords.y);

    if(Id == Mvt)
    {
        setMovementBorder((int (*)[_X_SIZE_])fill_tiles(e->coords, matrice, e->stat_mods[mv]), coorTab);
        setMovementZone(matrice, zone);
    }
    else
    {
        int range = get_range(e, Id);
        setActionBorder(e->coords, range,coorTab);
        setActionZone(range, coorTab, zone);
    }

    return (Coord *)zone;
}

bool Cast_check(action a, Coord coorTab[])
{
    printf("Verification de la porte...\n");
    Ability ab;
    int var;
    Entity * e = e_from_id(a.char_id);
    char log[STR_LONG];

    if(e->status_effect[Freezing])
    {
        sprintf(log, "%s is frozen and cannot do anything", e->cha_name);
        addLog(log);
        printf("Portee KO 2\n");
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
            printf("Portee KO 3\n");
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
            printf("Portee KO 4\n");
            return FALSE;
        }
    }

    if(tile_type(a.c, ab.target, e))
    {
        if(isInRange(coorTab, a.c))
        {
            printf("Portee OK\n");
            return TRUE;
        }
    }

    printf("Portee KO 5\n");

    return FALSE;
}