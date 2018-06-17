#include <string.h>
#include <stdio.h>
#include <switch.h>
#include <stdint.h>
#include "menu.hpp"
#include "helper.hpp"
#include "game.hpp"

int main(int argc, char **argv) {
    
    //Enable max-1080p support. Remove for 720p-only resolution.
    //gfxInitResolutionDefault();


    gfxInitDefault();
    //Set current resolution automatically depending on current/changed OperationMode. Only use this when using gfxInitResolution*().
    //gfxConfigureAutoResolutionDefault(true);
    
    Helper::State state = Helper::menu_s;

    Menu menu(&state);
    Game *game;

    while(appletMainLoop()) {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        if (state == Helper::menu_s) {
            menu.update(); 
        } else if (state == Helper::game_s) {
            game->update();
        } else if (state == Helper::ten) {
            game = new Game(9, 9, 10, &state);
        } else if (state == Helper::forty) {
            game = new Game(16, 16, 40, &state);
        } else if (state == Helper::ninetynine) {
            game = new Game(30, 16, 99, &state);
        }
        
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
