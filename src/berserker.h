Coord one_c = {0,0};
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
ab[0] = {Slash,1,0,1,{1,atk},1,&one_c,0,NULL,{0,0,0,0},NULL,{"Slash", "Slash at an ennemy."},"../inc/sprites/berserker/sprite_sheet/A1/"};
ab[1] = {Killing_Blow,2,1,1,{1,atk},1,&one_c,0,NULL,{0,0,0,0},NULL,{"Killing Blow","Slash at an ennemy, killing him increases attack permanently and gives one action point back."},"../inc/sprites/berserker/sprite_sheet/A2/"};
ab[3] = {Fury,2,2,0,{0,0},0,NULL,0,NULL,{0,0,0,0},NULL,{"Fury","Remove all debuffs and permanently increase attack by the number of turns removed."},"../inc/sprites/berserker/sprite_sheet/A3/"};
ab[4] = {Frenzied_Dash,3,4,9,{0.8,atk},31,ult,0,NULL,{0,0,0,0},NULL,{"Frenzied Dash","Jump to a tile, dealing aoe damage when landing."},"../inc/sprites/berserker/sprite_sheet/A4/"};

Class class = {Berserker,"Berserker",{13,8,10,10,0,10,10},{"Bloodlust","Killing two ennemies in the same turn refills action points and doubles mp and atk stats for one turn."},ab,"../inc/sprites/berserker/sprite_sheet/"};
