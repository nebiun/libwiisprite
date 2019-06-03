#include "global.h"

//Images
#include "_cursor_png.h"
#include "_point_png.h"
#include "_target_png.h"
#include "_target1_png.h"
#include "_wbb_png.h"

namespace Global
{
    wsp::GameWindow gwd;
    wsp::Image im_cursor,im_point,im_target,im_target1,im_wbb;
    Color verde,arancio,rosso;
    wsp::Sprite spr_cursor,spr_point,spr_wbb,spr_target,spr_target1;
    int tempo_fine,score;
};

using namespace Global;

void initRisorse(void)
{
    im_cursor.LoadImage(_cursor_png);
    im_point.LoadImage(_point_png);
    im_target.LoadImage(_target_png);
    im_target1.LoadImage(_target1_png);
    im_wbb.LoadImage(_wbb_png);

    spr_cursor.SetImage(&im_cursor);
    spr_point.SetImage(&im_point);
    spr_wbb.SetImage(&im_wbb);
    spr_target.SetImage(&im_target);
    spr_target1.SetImage(&im_target1);

    spr_wbb.SetPosition((LARGHEZZA_VIDEO-spr_wbb.GetWidth())/2,(ALTEZZA_VIDEO-spr_wbb.GetHeight())/2);

	verde.setColor(0,192,0);
	arancio.setColor(255,128,0);
	rosso.setColor(255,0,0);
}

int getTicks(void)
{
    return ticks_to_millisecs(gettime());
}
void ticks2clock(int t,int *m,int *s)
{
    *m=t/60000;
    *s=(t/1000)%60;
}

void calc_bary(Point points[],float a,float b,float c,float d,Point *barycentre)
{
    /*Pas besoin de tester si a+b+c+d!=0 car on l'a fait avant l'appel a cette fonction*/
    barycentre->x=(int)((float)(a*points[0].x+b*points[1].x+c*points[2].x+d*points[3].x)/(a+b+c+d));
    barycentre->y=(int)((float)(a*points[0].y+b*points[1].y+c*points[2].y+d*points[3].y)/(a+b+c+d));
}
