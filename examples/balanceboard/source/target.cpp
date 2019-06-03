#include <cmath>

#include "target.h"
#include "global.h"

using namespace Global;

int randMinMax(int min,int max)
{
    return rand()%(max-min+1)+min;
}

bool col_pt(Point p,Point p1,int w,int h)
{
    return p.x>=p1.x&&p.x<=p1.x+w&&p.y>=p1.y&&p.y<=p1.y+h;
}

Target::Target()
{
    reinit();
}
void Target::reinit()
{
    int dtempo=tempo_fine-getTicks();
    creation=getTicks();
    delay=(int)(.0336*dtempo+966.3866); //Acceleration au cours du tempo
    /* type=0 -> Bonus ; type=1 -> Malus */
    type=(rand()%4==0);//3 chances sur 4 que ce soit un bonus et 1 chance sur 4 que ce soit un malus

    int x_wbb=(LARGHEZZA_VIDEO-640)/2,y_wbb=(ALTEZZA_VIDEO-388)/2;

    pos.x=randMinMax(x_wbb+63+MARGE,x_wbb+63+516-48-MARGE);
    pos.y=randMinMax(y_wbb+43+MARGE,y_wbb+43+302-48-MARGE);
}
void Target::draw()
{
    if(getTicks()>=creation+delay) //Si le tempo d'apparition est ecoule
        reinit();

    if(type)
    {
        spr_target1.SetPosition(pos.x,pos.y);
        spr_target1.Draw(0,-31);
    }
    else
    {
        spr_target.SetPosition(pos.x,pos.y);
        spr_target.Draw(0,-31);
    }
}
bool Target::check(Point point)
{
    Point pts[4]={{point.x-16,point.y-16},
                  {point.x+16,point.y-16},
                  {point.x+16,point.y+16},
                  {point.x-16,point.y+16}};
    bool ret=false;
    for(int i=0;i<4;i++)
        ret=ret||col_pt(pts[i],pos,48,48);
    return ret;
}
bool Target::getType()
{
    return type;
}
