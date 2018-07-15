#ifndef GAME_HPP
#define GAME_HPP

#include <switch.h>
#include <stdint.h>
#include "blocked_bin.h"
#include "one_bin.h"
#include "two_bin.h"
#include "three_bin.h"
#include "four_bin.h"
#include "five_bin.h"
#include "six_bin.h"
#include "seven_bin.h"
#include "eight_bin.h"
#include "empty_bin.h"
#include "mine_bin.h"
#include "flag_bin.h"
#include "helper.hpp"
#include <vector>
#include <utility>
#include <stdlib.h> 
#include <ctime>
#include <time.h>
#include <stdio.h>
extern const ffnt_header_t tahoma24_nxfnt;
#define tahoma24 &tahoma24_nxfnt
#include "mines.hpp"
#include <string>

class Game {
    public:      
        Game(u32, u32, u32, Helper::State*);
        void update();
    private:
        u8* blocked_tile = (u8*)blocked_bin;
        u8* one_tile = (u8*)one_bin;
        u8* two_tile = (u8*)two_bin;
        u8* three_tile = (u8*)three_bin;
        u8* four_tile = (u8*)four_bin;
        u8* five_tile = (u8*)five_bin;
        u8* six_tile = (u8*)six_bin;
        u8* seven_tile = (u8*)seven_bin;
        u8* eight_tile = (u8*)eight_bin;
        u8* empty_tile = (u8*)empty_bin;
        u8* mine_tile = (u8*)mine_bin;
        u8* flagged_tile = (u8*)flag_bin;

        u32 h, w;
        u32 game_time = 0;
        Helper::State *state;
        std::pair<int, int> cursor_pos;
        Mines *mines;

        std::pair<int, int> pixelToPos(u32 x, u32 y);
        void render();        
        void renderBoard();        
        void renderCursor();
        bool checkCursorExists();
        void moveCursor(int xdir, int ydir); 
        u32 timeElapsed(u32 time);
        void startGameTimer();
        void renderTime();
};

#endif