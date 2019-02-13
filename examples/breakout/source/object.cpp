#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "object.h"

#include <wiisprite.h>

#include "_pallina_png.h"
#include "_barra_png.h"
#include "_mattoni_png.h"

using namespace wsp;

const int speed[2]={-4,4}; //Definition des constantes de speeds
bool vibr=false;

bool col_pt(int x,int y,Rectangle r)
{
    return x>=r.x&&x<=r.x+r.width&&y>=r.y&&y<=r.y+r.height; //Si le point (x;y) est dans le rectangle r
}
void vibrazione()
{
    static int tps=0;
    if(vibr&&ticks_to_millisecs(gettime())-tps>=100) //Si ça vibre et que ça fait plus de 100 ms
    {
        WPAD_Rumble(WPAD_CHAN_0,0);
        tps=ticks_to_millisecs(gettime());
        vibr=false;
    }

}

Object::Object(const unsigned char data[],wsp::LayerManager *manager,int x,int y,int w,int h):lmanager(manager)
{
    image.LoadImage(data);
    sprite.SetImage(&image,w,h);
    sprite.SetPosition(x,y);
    manager->Append(&sprite);
}
Object::~Object()
{
    image.DestroyImage();
    lmanager->Remove(&sprite);
}
void Object::posiziona(int x,int y)
{
    sprite.SetPosition(x,y);
}
bool Object::collision(Sprite *spr)
{
    return sprite.CollidesWith(spr);
}
int Object::getX()
{
    return sprite.GetX();
}
int Object::getY()
{
    return sprite.GetY();
}

Barra::Barra(int x,int y,LayerManager *manager):Object(_barra_png,manager,x,y)
{
}
Barra::~Barra()
{
}

Pallina::Pallina(int x,int y,LayerManager *manager):Object(_pallina_png,manager,x,y,64,64),esplosione(false),tps(0),frame(0)
{
    sprite.DefineCollisionRectangle(24,24,16,16); //Rimuoviamo la parte che conterra` l'esplosione
}
Pallina::~Pallina()
{
}
void Pallina::def_speed(int i0,int i1)
{
    v[0]=speed[i0];
    v[1]=speed[i1];
}
bool Pallina::exploser()
{
    if(!esplosione)
    {
        esplosione=true;
        tps=ticks_to_millisecs(gettime());
        return true;
    }
    return false;
}
void Pallina::refresh(Barra *barra,bool launched,int x_ir,Mattone *objets[])
{
    if(esplosione&&ticks_to_millisecs(gettime())-tps>=100)
    {
        frame++;
        if(frame>3)
        {
            esplosione=false;
            frame=0;
            int cX=sprite.GetX()+32,cY=sprite.GetY()+32;
            for(int i=0;i<N_MATTONI;i++)
            {
                int cX1=objets[i]->getX()+16,cY1=objets[i]->getY()+8;
                if((cX1-cX)*(cX1-cX)+(cY1-cY)*(cY1-cY)<=64*64)
                    objets[i]->impact();
            }
        }
        sprite.SetFrame(frame);
        tps=ticks_to_millisecs(gettime());
    }

    if(!launched) //Se la pallina deve rimanere agganciata alla barra
        sprite.SetPosition(barra->getX(),480-16-16-24); //Centriamo la pallina sulla racchetta
    else
        sprite.Move(v[0],v[1]); //La pallina viene spostata relativamente alla sua velocita` nella X e Y

    /*Testiamo gli overflow*/
    if(sprite.GetX()<0)
    {
        sprite.SetPosition(0,sprite.GetY());
        v[0]=speed[1];
    }
    else if(sprite.GetX()+24+16>640)
    {
        sprite.SetPosition(640-24-16,sprite.GetY());
        v[0]=speed[0];
    }
    if(sprite.GetY()<0)
    {
        sprite.SetPosition(sprite.GetX(),0);
        v[1]=speed[1];
    }
}
void Pallina::impacts(Mattone *objets[],Barra *barra)
{
    for(int i=0;i<N_MATTONI;i++)
        if(objets[i]->getVie()&&objets[i]->collision(&sprite))
        {
            Rectangle col={objets[i]->getX(),objets[i]->getY(),32,16};
            if(col_pt(sprite.GetX()+24,sprite.GetY()+24,col)||col_pt(sprite.GetX()+24+16,sprite.GetY()+24,col))
                v[1]*=-1;
            if(col_pt(sprite.GetX()+24,sprite.GetY()+24+16,col)||col_pt(sprite.GetX()+24+16,sprite.GetY()+24+16,col))
                v[0]*=-1;
            objets[i]->impact();
            break;
        }

    if(barra->collision(&sprite))
        v[1]=speed[0];
}

Mattone::Mattone(int x,int y,wsp::LayerManager *manager,int frame):Object(_mattoni_png,manager,x,y,32,16),attivo(true)
{
    sprite.SetFrame(frame);
}
Mattone::~Mattone()
{
}
bool Mattone::getVie()
{
    return attivo;
}
void Mattone::impact()
{
    attivo=false;
    sprite.SetVisible(false); //Mettiamo il mattone invisibile, eviteremo le complicazioni della rimozione dello sprite

    WPAD_Rumble(WPAD_CHAN_0,1); //Facciamo vibrare il Wiimote
    vibr=true; //Non dimentichiamo di mettere la variabile globale di vibrazione a true
}
