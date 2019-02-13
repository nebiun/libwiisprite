#ifndef __OBJECT__
#define __OBJECT__

#include <wiisprite.h>

#define N_MATTONI 120 //Mattoni totali

extern const int speed[2]; //Costanti di velocita`
extern bool vibr; //Stato della vibrazione

bool col_pt(int x,int y,wsp::Rectangle r); //Collisione con un punto
void vibrazione(); //Verifica la vibrazione e fallo se necessario

class Object
{
    public:
        Object(const unsigned char data[],wsp::LayerManager *manager,int x,int y,int w=0,int h=0);
        ~Object();
        void posiziona(int x,int y);
        bool collision(wsp::Sprite *spr);
        int getX();
        int getY();
    protected:
        wsp::Image image;
        wsp::Sprite sprite;
        wsp::LayerManager *lmanager;
};

class Barra:public Object
{
    public:
        Barra(int x,int y,wsp::LayerManager *manager);
        ~Barra();
};

class Mattone:public Object
{
    public:
        Mattone(int x,int y,wsp::LayerManager *manager,int frame);
        ~Mattone();
        bool getVie();
        void impact();
    private:
        bool attivo; //True se (ancora) presente
};

class Pallina:public Object
{
    public:
        Pallina(int x,int y,wsp::LayerManager *manager);
        ~Pallina();
        void def_speed(int i0,int i1);
        bool exploser();
        void refresh(Barra *barra,bool launched,int x_ir,Mattone *objets[]);
        void impacts(Mattone *objets[],Barra *barra);
    private:
        bool esplosione;
        int v[2],	//Velocita` della palla
            tps,	//Tempo per le esplosioni
            frame; 	//Frame attuale
};

#endif
