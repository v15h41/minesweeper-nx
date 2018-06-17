#include <string.h>
#include <stdio.h>
#include <switch.h>
#include <stdint.h>
#include "game.hpp"

int main(int argc, char **argv) {
    //Enable max-1080p support. Remove for 720p-only resolution.
    //gfxInitResolutionDefault();

    gfxInitDefault();
    //Set current resolution automatically depending on current/changed OperationMode. Only use this when using gfxInitResolution*().
    //gfxConfigureAutoResolutionDefault(true);

    while(appletMainLoop()) {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu
       
        Game game;
        game.update();        

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
