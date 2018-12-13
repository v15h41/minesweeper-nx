#include <string.h>
#include <stdio.h>
#include <switch.h>
#include <stdint.h>
#include "menu.hpp"
#include "helper.hpp"
#include "game.hpp"

int main(int argc, char **argv) {    
    gfxInitDefault();

    // store the current state
    Helper::State state = Helper::menu_s;

    // menu object
    Menu menu(&state);
    // game object
    Game *game;

    while(appletMainLoop()) {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        // break in order to return to hbmenu
        if (kDown & KEY_PLUS) break; 

        // render the screen based on the current state 
        if (state == Helper::menu_s) {
            menu.update(); 
        } else if (state == Helper::game_s) {
            game->update();
        // initialize the game object 
        } else if (state == Helper::ten) {
            game = new Game(9, 9, 10, &state);
        } else if (state == Helper::forty) {
            game = new Game(16, 16, 40, &state);
        } else if (state == Helper::ninetynine) {
            game = new Game(16, 30, 99, &state);
        }
        
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
