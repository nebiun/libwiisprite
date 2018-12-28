/*
	Wiisprite API Example by Feesh!
	Invaders... Possibly from Space!
*/


#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <wiisprite.h>

#include "engine.h"

using namespace wsp;

bool quit = false;

int main(int argc, char **argv)
{
	GameWindow gw;

	gw.InitVideo();
	Quad fade;
	fade.SetWidth(640); // Used at the end of the prog
	fade.SetHeight(480);
	GXColor fade_col = { 0,0,0,0 };
	fatInitDefault();
	
	WPAD_Init();
	WPAD_SetVRes(WPAD_CHAN_ALL,640,480);
    WPAD_SetDataFormat(WPAD_CHAN_0,WPAD_FMT_BTNS_ACC_IR);
	
	Engine engine;

	for(;;)
	{
		u32 btn;
		WPAD_ScanPads();
		
		btn = WPAD_ButtonsDown(WPAD_CHAN_0);
		
		if(btn & WPAD_BUTTON_HOME)
		{
			quit = true;
		}
		if(!((*(u32*)0xCC003000)>>16))
		{
			quit = true;
		}		
		
		engine.update();
		if(quit)
		{
			if(fade_col.a<255)
			{
				fade_col.a+=5;
			}
			fade.SetFillColor(fade_col);
			fade.Draw();
			if(fade_col.a==255)
				break;
		}
		gw.Flush();
	}
	return 0;
}
