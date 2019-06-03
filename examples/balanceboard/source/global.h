#ifndef __GLOBAL__
#define __GLOBAL__

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>
#include <ft2build.h>
#include <wiisprite.h>

#define LARGHEZZA_VIDEO 640
#define ALTEZZA_VIDEO 480
#define TEMPO_MAX (2*60*1000)

struct Point
{
    int x,y;
};

namespace Global
{
    extern wsp::GameWindow gwd;
    extern wsp::Image im_cursor,im_point,im_target,im_target1,im_wbb;
    extern Color verde,arancio,rosso;
    extern wsp::Sprite spr_cursor,spr_point,spr_wbb,spr_target,spr_target1;
    extern int tempo_fine,score;
};

void initRisorse(); //Fonction qui nitialise toutes les ressources nécessaires (ici les images et les sprites)
int getTicks(); //Fonction qui renvoie l'heure comme time(NULL)
void ticks2clock(int t,int *m,int *s); //Fonction qui transforme des ticks en minutes et secondes
void calc_bary(Point points[],float a,float b,float c,float d,Point *barycentre); //Fonction qui calcule le barycentre de 4 points

#endif
