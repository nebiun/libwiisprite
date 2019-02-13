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
        
    bool continuare=true;

    gwd.InitVideo();

    im.LoadImage(barra_png);
    sprite.SetImage(&im);
    sprite.SetPosition(0, 0);

    manager.Append(&sprite);

    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR); 

    while(continuare)
    {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        if(pressed & WPAD_BUTTON_HOME)
            continuare=false;
        manager.Draw(0, 0);
        gwd.Flush();
    }
 
    return 0;
}
