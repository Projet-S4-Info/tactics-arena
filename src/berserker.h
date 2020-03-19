Coord * one_c = malloc(sizeof(Coord));
Damage * one = malloc(sizeof(Damage));
Damage * FD = malloc(sizeof(Damage));
*one_c = {0,0};
*one = {1,atk};
*FD = {0.8,atk};
Coord ult[31];
int i,x,y;
for(i=0,x=-2,y=-4;x<=2;)
{
    if(!(x==-2&&y==-3||x==-2&&y==3||x==2&&y==-3||x==2&&y==3))
    {
        ult[i].x=x;
        ult[i].y=y;
        i++;
    }
    if(y=3)
    {
        y=-3;
        x++;
    }
    else y++;
}

abilities ab[4];
ab[0] = {Slash,1,0,1,one,1,one_c,0,NULL,NULL,NULL,{"Slash", "Slash at an ennemy."},"../inc/sprites/berserker/sprite_sheet/A1/"};
ab[1] = {Killing_Blow,2,1,1,one,1,one_c,0,NULL,NULL,NULL,{"Killing Blow","Slash at an ennemy, killing him increases attack permanently and gives one action point back."},"../inc/sprites/berserker/sprite_sheet/A2/"};
ab[3] = {Fury,2,2,0,NULL,0,NULL,0,NULL,NULL,NULL,{"Fury","Remove all debuffs and permanently increase attack by the number of turns removed."},"../inc/sprites/berserker/sprite_sheet/A3/"};
ab[4] = {Frenzied_Dash,3,4,9,FD,31,ult,0,NULL,NULL,NULL,{"Frenzied Dash","Jump to a tile, dealing aoe damage when landing."},"../inc/sprites/berserker/sprite_sheet/A4/"};

Class class = {Berserker,"Berserker",{13,8,10,10,0,10,10},{"Bloodlust","Killing two ennemies in the same turn refills action points and doubles mp and atk stats for one turn."},ab,"../inc/sprites/berserker/sprite_sheet/"};
