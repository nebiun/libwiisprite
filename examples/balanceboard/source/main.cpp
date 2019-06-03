#include <stdio.h>
#include <stdlib.h>

#include "gioco.h"

using namespace Global;

int main(int argc,char *argv[])
{
    srand(ticks_to_secs(gettick()));

    WPAD_Init();
    
    WPAD_SetVRes(WPAD_CHAN_ALL,640,480);
    WPAD_SetDataFormat(WPAD_CHAN_0,WPAD_FMT_BTNS_ACC_IR);

    gwd.InitVideo();

    initRisorse();

    if(connect_board())
        while(1)
        {
            if(!countdown())
                break;
            if(!gioco())
                break;
            if(!fine())
                break;
        }

    return 0;
}
