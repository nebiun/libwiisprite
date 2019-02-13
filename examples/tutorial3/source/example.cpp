#include <stdio.h>
#include <stdlib.h>

#include <gccore.h>
#include <wiiuse/wpad.h>

#include <wiisprite.h>

#include "tileset_png.h"

#define LARGHEZZA_SCHERMO 640      //Larghezza dello schermo
#define ALTEZZA_SCHERMO   480      //Altezza dello schermo
#define TILES_X (LARGHEZZA_SCHERMO>>5) //Numero di tile in X
#define TILES_Y (ALTEZZA_SCHERMO>>5)   //Numero di tile in Y

using namespace wsp;

int main(int argc, char **argv)
{
    s32 map_c[TILES_X*TILES_Y]=
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,1,2,2,2,3,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,4,2,2,2,2,2,5,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    GameWindow gwd;
    Image im;
    TiledLayer map(TILES_X,TILES_Y,0);
    LayerManager manager(1);

    bool continuare=true;

    gwd.InitVideo();

    im.LoadImage(tileset_png);
    map.SetStaticTileset(&im,32,32);
    for(int i=0;i<TILES_X*TILES_Y;i++)
        map.SetCell(i%TILES_X,i/TILES_X,map_c[i]); //i%TILES_X => x ; i/TILES_X => y

    manager.Append(&map);

    WPAD_Init();

    while(continuare)
    {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if(pressed & WPAD_BUTTON_HOME)
            continuare=false;
        manager.Draw(0, 0);
            gwd.Flush();
    }

    return 0;
}