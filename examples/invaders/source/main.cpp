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
u32 _button_pressed;
u32 _button_held;

void ScanPADSandReset(void)
{
	
	WPAD_ScanPads();
	_button_pressed = WPAD_ButtonsDown(0);
	_button_held = WPAD_ButtonsHeld(0);
	
	if(_button_pressed & WPAD_BUTTON_HOME)
	{
		quit = true;
	}
	if(!((*(u32*)0xCC003000)>>16))
	{
		quit = true;
	}
}


int main(int argc, char **argv)
{
	GameWindow gw;

	gw.InitVideo();
	WPAD_Init();
	Quad fade;
	fade.SetWidth(640); // Used at the end of the prog
	fade.SetHeight(480);
	GXColor fade_col = { 0,0,0,0 };
	fatInitDefault();
	
	Engine engine;

	for(;;)
	{
		ScanPADSandReset();
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
