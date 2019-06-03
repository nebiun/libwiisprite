#include "gioco.h"
#include "target.h"

//Font
#include "cheesebu_ttf.h"

using namespace Global;

bool connect_board(void)
{
    Sprite txt;
    bool continua=true;

    ftImage im_txt(640,480);
    im_txt.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt.setSize(40);
    im_txt.setColor(verde);
    im_txt.printf("Connetti la Wii Balance Board\n              e premi A");
    im_txt.flush();

    txt.SetImage(&im_txt);
    txt.SetPosition(70,(ALTEZZA_VIDEO-100)/2);

    gwd.SetBackground((GXColor){255,255,255,255});

    while(continua)
    {
        WPAD_ScanPads();

        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        if(pressed & WPAD_BUTTON_HOME)
            continua=false;
        else if(pressed & WPAD_BUTTON_A)
            break;

        GX_SetScissorBoxOffset(0,-31);
        GX_SetScissor(0,0,900,800);

        txt.Draw(0,-31);

        GX_SetScissorBoxOffset(0,0);
        GX_SetScissor(0,0,wsp::GameWindow::GetWidth(),wsp::GameWindow::GetHeight());

        gwd.Flush();
    }

    return continua;
}

bool countdown(void)
{
    Sprite txt_score,txt_tempo,txt_countdown;

    bool continua=true;
    int n=3;
    float z=6.;
    int m,s;

    ftImage im_txt_score(480,48),im_txt_tempo(240,48),im_txt_countdown(40,48);

    ticks2clock(TEMPO_MAX,&m,&s);

    im_txt_score.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_score.setSize(40);
    im_txt_score.setColor(verde);
    im_txt_score.printf("Score : 000");
    im_txt_score.flush();

    im_txt_tempo.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_tempo.setSize(40);
    im_txt_tempo.setColor(verde);
    im_txt_tempo.printf("%02d:%02d",m,s);
    im_txt_tempo.flush();

    im_txt_countdown.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_countdown.setSize(40);
    im_txt_countdown.setColor(verde);
    im_txt_countdown.printf("%d",n);
    im_txt_countdown.flush();

    txt_score.SetImage(&im_txt_score);
    txt_tempo.SetImage(&im_txt_tempo);
    txt_countdown.SetImage(&im_txt_countdown);

    txt_score.SetPosition(0,0);
	txt_tempo.SetPosition(LARGHEZZA_VIDEO-100,ALTEZZA_VIDEO-10-50);
    txt_countdown.SetPosition(LARGHEZZA_VIDEO/2-15,ALTEZZA_VIDEO/2-30);

    gwd.SetBackground((GXColor){0,0,0,255});

    int tempo=getTicks();

    while(continua&&getTicks()-tempo<=3000)
    {
        WPAD_ScanPads();

        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        if(pressed & WPAD_BUTTON_HOME)
            continua=false;

        txt_countdown.SetZoom(z);

        GX_SetScissorBoxOffset(0,-31);
        GX_SetScissor(0,0,900,800);

        spr_wbb.Draw(0,-31);
        txt_countdown.Draw(0,-31);
        txt_score.Draw(0,-31);
        txt_tempo.Draw(0,-31);

        GX_SetScissorBoxOffset(0,0);
        GX_SetScissor(0,0,wsp::GameWindow::GetWidth(),wsp::GameWindow::GetHeight());

        gwd.Flush();

        z-=.1;
        if(n!=3-(getTicks()-tempo)/1000) //3-(getTicks()-tempo)/1000 consente di fare 3->0
        {
            z=6.;
            n=3-(getTicks()-tempo)/1000;

            im_txt_countdown.clear();
            im_txt_countdown.reset();
            im_txt_countdown.printf("%d",n);
            im_txt_countdown.flush();
        }
    }

    return continua;
}

bool gioco(void)
{
    Sprite txt_score,txt_tempo;

    ftImage im_txt_score(480,48),im_txt_tempo(240,48);

    im_txt_score.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_score.setSize(40);
    im_txt_score.setColor(verde);
	im_txt_score.printf("Score : 000");
    im_txt_score.flush();

    im_txt_tempo.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_tempo.setSize(40);
    im_txt_tempo.setColor(verde);
	im_txt_tempo.printf("**:**");
    im_txt_tempo.flush();

    txt_score.SetImage(&im_txt_score);
    txt_tempo.SetImage(&im_txt_tempo);

    spr_point.SetPosition(LARGHEZZA_VIDEO/2,ALTEZZA_VIDEO/2);

    tempo_fine=getTicks()+TEMPO_MAX;	//Variable dichiarata nel Global namespace
    score=0;							//Idem

	txt_score.SetPosition(0,0);
	txt_tempo.SetPosition(LARGHEZZA_VIDEO-100,ALTEZZA_VIDEO-10-50);

    Target *target=new Target();
    Point pos_wbb={spr_wbb.GetX(),spr_wbb.GetY()};
    Point points[4]={
                        {pos_wbb.x+63,pos_wbb.y+43},         //Point A
                        {pos_wbb.x+63+516,pos_wbb.y+43},     //Point B
                        {pos_wbb.x+63+516,pos_wbb.y+43+302}, //Point C
                        {pos_wbb.x+63,pos_wbb.y+43+302},     //Point D
                    };                                       //Questi punti corrispondono ai vertici della Balance Board
    Point barycentre={-1,-1};
    struct expansion_t exp;
	bool continua=true;
	bool visible=true;

    gwd.SetBackground((GXColor){0,0,0,255});

    while(continua)
    {
        WPAD_ScanPads();
        WPAD_Expansion(WPAD_BALANCE_BOARD,&exp);

        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        if(pressed & WPAD_BUTTON_HOME)
            continua=false;

        float a=exp.wb.tl,
              b=exp.wb.tr,
              c=exp.wb.br,
              d=exp.wb.bl;
        /* Abbiamo un peso, quindi nessun valore negativo; e divisione per 0 impossibile 
		 *(funzione cf "calc_bary"); allo stesso tempo metto una base a 5 kg per evitare 
		 * i valori parassiti quando non siamo sulla Balance Board
		 */
		if(a>=0&&b>=0&&c>=0&&d>=0&&a+b+c+d>=5) 
		{
            calc_bary(points,a,b,c,d,&barycentre);
            visible=true;
            spr_point.SetPosition(barycentre.x-16,barycentre.y-16);

            if(target->check(barycentre))
            {
                if(target->getType()) //Malus
                {
                    score-=2;
                    if(score<0)
                        score=0;
                    tempo_fine-=10000; //10 secondi di penalita`
                }
                else
                    score++;

                im_txt_score.clear();
                im_txt_score.reset();
                im_txt_score.printf("Score : %03d",score);
                im_txt_score.flush();

                target->reinit();
            }
        }
        else
            visible=false;
	
        int dtempo=tempo_fine-getTicks(),m,s;
        if(dtempo<0)
            break;

        ticks2clock(dtempo,&m,&s);

        im_txt_tempo.clear();
        im_txt_tempo.reset();
        im_txt_tempo.printf("%02d:%02d",m,s);
        if(m==0&&s<=10)      //Se rimangono meno di 10 secondi
            im_txt_tempo.setColor(rosso);
        else if(m==0&&s<=30) //Se rimangono tra gli 11 e i 30 secondi
            im_txt_tempo.setColor(arancio);
        else
            im_txt_tempo.setColor(verde);
        im_txt_tempo.flush();

        GX_SetScissorBoxOffset(0,-31);
        GX_SetScissor(0,0,900,800);

        spr_wbb.Draw(0,-31);
        spr_point.Draw(0,-31);

        target->draw();

        txt_score.Draw(0,-31);
        txt_tempo.Draw(0,-31);

        GX_SetScissorBoxOffset(0,0);
        GX_SetScissor(0,0,wsp::GameWindow::GetWidth(),wsp::GameWindow::GetHeight());

        gwd.Flush();
    }

    delete target;

    return continua;
}

bool fine(void)
{
    Sprite txt_score,txt_rigiocare,txt_terminare;
    ftImage im_txt_score(480,48),im_txt_rigiocare(640,480),im_txt_terminare(640,480);

    im_txt_score.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_score.setSize(40);
    im_txt_score.setColor(verde);
    im_txt_score.printf("Score : %03d",score);
    im_txt_score.flush();

    im_txt_rigiocare.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_rigiocare.setSize(72);
    im_txt_rigiocare.setColor(verde);
    im_txt_rigiocare.printf("");
    im_txt_rigiocare.flush();

    im_txt_terminare.setFont(cheesebu_ttf,cheesebu_ttf_size);
    im_txt_terminare.setSize(72);
    im_txt_terminare.setColor(verde);
    im_txt_terminare.printf("");
    im_txt_terminare.flush();

    txt_score.SetImage(&im_txt_score);
    txt_rigiocare.SetImage(&im_txt_rigiocare);
    txt_terminare.SetImage(&im_txt_terminare);

    spr_cursor.SetPosition(LARGHEZZA_VIDEO,ALTEZZA_VIDEO);

    bool continua=true;
    int choix=0;
    struct ir_t ir;

    txt_score.SetPosition((LARGHEZZA_VIDEO-200)/2,20);
    txt_rigiocare.SetPosition((LARGHEZZA_VIDEO-222)/2,ALTEZZA_VIDEO/2-20-75);
    txt_terminare.SetPosition((LARGHEZZA_VIDEO-217)/2,ALTEZZA_VIDEO/2+20);

    Point pos_txt_rigiocare={txt_rigiocare.GetX(),txt_rigiocare.GetY()},
          pos_txt_terminare={txt_terminare.GetX(),txt_terminare.GetY()};

    gwd.SetBackground((GXColor){255,255,255,255});

    while(continua)
    {
        WPAD_ScanPads();
        WPAD_IR(WPAD_CHAN_0,&ir);

        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        if(pressed & WPAD_BUTTON_HOME)
            continua=false;
        else if(pressed & WPAD_BUTTON_A)
        {
            if(choix==1)
                break;
            else if(choix==2)
                continua=false;
        }

        Point pos_cursor={(int)ir.x,(int)ir.y};

        spr_cursor.SetPosition(pos_cursor.x,pos_cursor.y);

        if(pos_cursor.x>=pos_txt_rigiocare.x&&pos_cursor.x<=pos_txt_rigiocare.x+222&&pos_cursor.y>=pos_txt_rigiocare.y&&pos_cursor.y<=pos_txt_rigiocare.y+75)      //Si le courseur est sur Rigiocare
            choix=1;
        else if(pos_cursor.x>=pos_txt_terminare.x&&pos_cursor.x<=pos_txt_terminare.x+217&&pos_cursor.y>=pos_txt_terminare.y&&pos_cursor.y<=pos_txt_terminare.y+75) //Si le curseur est sur Terminare
            choix=2;
        else
            choix=0;

        GX_SetScissorBoxOffset(0,-31);
        GX_SetScissor(0,0,900,800);

        txt_score.Draw(0,-31);

        im_txt_rigiocare.clear();
        im_txt_rigiocare.reset();
        im_txt_rigiocare.setColor(choix==1?arancio:verde); //Se selezionato in verde, altrimenti arancio
        im_txt_rigiocare.printf("Rigiocare");
        im_txt_rigiocare.flush();
        txt_rigiocare.Draw(0,-31);

        im_txt_terminare.clear();
        im_txt_terminare.reset();
        im_txt_terminare.setColor(choix==2?arancio:verde); //Idem
        im_txt_terminare.printf("Terminare");
        im_txt_terminare.flush();
        txt_terminare.Draw(0,-31);

        spr_cursor.Draw(0,-31);

        GX_SetScissorBoxOffset(0,0);
        GX_SetScissor(0,0,wsp::GameWindow::GetWidth(),wsp::GameWindow::GetHeight());

        gwd.Flush();
    }

    return continua;
}
