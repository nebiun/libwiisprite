#include <stdio.h>
#include <stdlib.h>

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include <wiisprite.h>

#include "_pallina_ico_png.h"
#include "_esplosione_ico_png.h"
#include "_vinto_png.h"
#include "_perso_png.h"
#include "_riprova_png.h"
#include "_esci_png.h"
#include "_cursore_png.h"

#include "object.h"

using namespace wsp;

/*
Funzione gioco: gameplay
Argomento gwd: puntatore a GameWindow (puntatore per evitare la duplicazione della variabile
valore di ritorno: -1 => perso; 0 => lasciare; 1 => vinto
*/
int gioco(GameWindow *gwd);

/*
Funzione replay: interfaccia che chiede di riprovare o uscire, mentre mostra se uno ha vinto o perso
Argomento gwd: puntatore a GameWindow (puntatore per evitare la duplicazione della variabile
argomento ret_gioco: valore di ritorno della funzione di gioco
valore di ritorno: true => replay; false => esci
*/
bool replay(GameWindow *gwd,int ret_gioco);

int main(int argc, char **argv)
{
    GameWindow gwd;
    int ret_gioco;
    bool ret_replay;
    
    srand(ticks_to_secs(gettime())); //Inizializziamo il generatore di numeri pseudo casuali

    gwd.InitVideo();

    WPAD_Init();
    WPAD_SetVRes(WPAD_CHAN_ALL,640,480);
    WPAD_SetDataFormat(WPAD_CHAN_0,WPAD_FMT_BTNS_ACC_IR);

    do
    {
        ret_gioco=gioco(&gwd);
        if(ret_gioco!=0) //Se non siamo partiti nel gioco
            ret_replay=replay(&gwd,ret_gioco);
    }while(ret_replay&&ret_gioco!=0); //Finche` non decidiamo se rifare o uscire dal gioco

    return 0;
}

int gioco(GameWindow *gwd)
{
    int ret=0;

    Barra *barra=NULL;
    Pallina *pallina=NULL;
    Mattone *objets[N_MATTONI]={NULL};
    Sprite spr_pallina_ico[3],spr_esplosione_ico[3]; //Sprites icone (pallina e esplosione)
    LayerManager manager(N_MATTONI //Mattoni
                         +2 //Barra et pallina
                         +3 //Icones pallina
                         +3 //Icones esplosione
                        );
    Image im_pallina_ico,im_esplosione_ico;
    
    int n_palline=0,n_esplosioni=0;

    bool continuare=true,launched=false; //launched indique si on a lance la pallina (true) ou pas (false)

    im_pallina_ico.LoadImage(_pallina_ico_png);
    im_esplosione_ico.LoadImage(_esplosione_ico_png);

    for(int i=0;i<3;i++)
    {
        spr_pallina_ico[i].SetImage(&im_pallina_ico);
        spr_esplosione_ico[i].SetImage(&im_esplosione_ico);

        /*Piccoli calcoli di posizione (prendi un foglio :)*/
        spr_pallina_ico[i].SetPosition(-20*i+48,0);
        spr_esplosione_ico[i].SetPosition(640-16+20*i-48,0);

        manager.Append(&spr_pallina_ico[i]);
        manager.Append(&spr_esplosione_ico[i]);
    }

    barra=new Barra((640-64)/2,480-16,&manager);
    pallina=new Pallina((640-16)/2,480-16-16,&manager);
    for(int i=0;i<N_MATTONI;i++)
    {
        int x=i%12;
        int y=i/12;
        objets[i]=new Mattone((x+1)*48-8,(y+1)*32,&manager,rand()%5);
    }

    while(continuare)
    {
        struct ir_t ir;
        int x_ir;
        WPAD_ScanPads();

        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        if(pressed & WPAD_BUTTON_HOME)
            continuare=false;
        else if(pressed & WPAD_BUTTON_A)
        {
            if(!launched)
            {
                launched=true;
                pallina->def_speed(rand()%2,0); //Definiamo solo casualmente la velocità in X, la velocita` in Y deve essere negativa per salire
            }
        }
        else if(pressed & WPAD_BUTTON_B)
        {
            if(n_esplosioni<3&&pallina->exploser()) //Se ci sono ancora esplosioni e la pallina non sta gia` esplodendo
                spr_esplosione_ico[n_esplosioni++].SetVisible(false);
        }

        WPAD_IR(WPAD_CHAN_0,&ir);

        x_ir=(int)ir.x;
        if(x_ir<64)
            x_ir=64;
        else if(x_ir>640-64)
            x_ir=640-64;

        barra->posiziona(x_ir-24,480-16); //Spostiamo la barra in base alla posizione del Wiimote
        pallina->refresh(barra,launched,x_ir,objets);

        if(pallina->getY()>=480) //Se la pallina va "sotto" lo schermo
        {
            launched=false;
            if(n_palline<2)
                spr_pallina_ico[n_palline++].SetVisible(false);
            else //Game Over ;)
            {
                ret=-1;
                continuare=false;
            }
        }

        pallina->impacts(objets,barra); //Verifichiamo gli impatti

        bool successo=true;
        for(int i=0;i<N_MATTONI;i++)
            if(objets[i]->getVie()) //Se ci sono ancora mattoni, non vinciamo
            {
                successo=false;
                break;
            }
        if(successo) //Se non ci sono più mattoni lasciamo il ciclo
        {
            ret=1;
            continuare=false;
        }

        if(vibr) //Se il Wiimote sta vibrando, interrompiamo la vibrazione dopo 100 ms
            vibrazione();

        manager.Draw(0, 0);
        gwd->Flush();
    }
    /*Liberazione della memoria*/
    delete barra;
    delete pallina;
    for(int i=0;i<N_MATTONI;i++)
        delete objets[i];
    manager.RemoveAll();
    return ret;
}

bool replay(GameWindow *gwd,int ret_gioco)
{
    bool ret=false;

    LayerManager manager(1 //Vinto/Perso
                         +2 //Riprova e Esci
                         +1 //Cursore
                        );
    Image im,im_riprova,im_esci,im_cursore;
    Sprite spr,spr_riprova,spr_esci,spr_curseur;
    bool continuare=true;

    im.LoadImage(ret_gioco==1?_vinto_png:_perso_png);
    im_riprova.LoadImage(_riprova_png);
    im_esci.LoadImage(_esci_png);
    im_cursore.LoadImage(_cursore_png);

    spr.SetImage(&im);
    spr.SetPosition(192,96);

    spr_riprova.SetImage(&im_riprova);
    spr_riprova.SetPosition(16,288);

    spr_esci.SetImage(&im_esci);
    spr_esci.SetPosition(368,288);

    spr_curseur.SetImage(&im_cursore);

    manager.Append(&spr_curseur);
    manager.Append(&spr);
    manager.Append(&spr_riprova);
    manager.Append(&spr_esci);

    while(continuare)
    {
        struct ir_t ir;
        WPAD_ScanPads();
        WPAD_IR(WPAD_CHAN_0,&ir);

        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        if(pressed & WPAD_BUTTON_HOME)
            continuare=false;
        else if(pressed & WPAD_BUTTON_A)
        {
            Rectangle col_riprova={spr_riprova.GetX(),spr_riprova.GetY(),spr_riprova.GetWidth(),spr_riprova.GetHeight()},
                      col_esci={spr_esci.GetX(),spr_esci.GetY(),spr_esci.GetWidth(),spr_esci.GetHeight()};
            if(col_pt((int)ir.x,(int)ir.y,col_riprova)) //Se clicca su "Riprova"
            {
                ret=true;
                continuare=false;
            }
            else if(col_pt((int)ir.x,(int)ir.y,col_esci)) //Se clicca su "Esci"
                continuare=false;
        }

        spr_curseur.SetPosition((int)ir.x,(int)ir.y);

        manager.Draw(0, 0);
        gwd->Flush();
    }
    manager.RemoveAll();
    return ret;
}
