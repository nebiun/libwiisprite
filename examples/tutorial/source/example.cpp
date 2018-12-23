#include <stdio.h>
#include <stdlib.h>

#include <gccore.h>
#include <wiiuse/wpad.h>

#include <wiisprite.h>

#include "barra_png.h"

using namespace wsp;

int main(int argc, char **argv)
{
    GameWindow gwd;
    Image im;
    Sprite sprite;
    LayerManager manager(1);
        
    bool continuer=true;

    gwd.InitVideo();

    im.LoadImage(barra_png);
    sprite.SetImage(&im);
    sprite.SetPosition(0, 0);

    manager.Append(&sprite);

    WPAD_Init();

    while(continuer)
    {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if(pressed & WPAD_BUTTON_HOME)
            continuer=false;
        manager.Draw(0, 0);
        gwd.Flush();
    }
 
    return 0;
}