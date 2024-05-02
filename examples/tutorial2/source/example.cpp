#include <stdio.h>
#include <stdlib.h>
#include <cmath> //Includiamo la libreria matematica che consente di avere la tangente

#include <gccore.h>
#include <wiiuse/wpad.h>

#include <wiisprite.h>

#include "mattoni_png.h"

using namespace wsp;

int main(int argc, char **argv)
{
	GameWindow gwd;
	Image im;
	Sprite sprite;
	LayerManager manager(1);
//	int frame=0;
	float angle=0;
//	struct ir_t ir;

	bool continuare=true;

	gwd.InitVideo();

	im.LoadImage(mattoni_png);
	sprite.SetImage(&im,32,16);
	//Centriamo lo sprite
	sprite.SetPosition((640-sprite.GetWidth())/2,(480-sprite.GetHeight())/2); 

	manager.Append(&sprite);

	WPAD_Init();
//	WPAD_SetVRes(WPAD_CHAN_ALL,640,480);
	WPAD_SetDataFormat(WPAD_CHAN_0,WPAD_FMT_BTNS_ACC_IR);

	while(continuare)
	{
		WPAD_ScanPads();
	//	WPAD_IR(WPAD_CHAN_0,&ir);
		u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
		if(pressed & WPAD_BUTTON_HOME)
			continuare=false;
		else if(pressed & WPAD_BUTTON_LEFT)
		{
			frame--;
			if(frame<0)  //Se siamo prima del primo frame 
				frame=3; //Iniziamo dall'ultimo frame
		}
		else if(pressed & WPAD_BUTTON_RIGHT)
		{
			frame++;
			frame%=4; //Se siamo all'ultimo frame, iniziamo dal primo frame
		}
		sprite.SetFrame(frame);
		
		//Non dimenticare di convertire in gradi
	//	angle=180.*atan2(ir.y-sprite.GetY(),ir.x-sprite.GetX())/M_PI; 
	//	sprite.SetRotation(angle/2.);
		manager.Draw(0, 0);
		gwd.Flush();
	}

	return 0;
}