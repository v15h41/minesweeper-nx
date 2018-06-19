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
#include <stdio.h>

class Game {
    public:
        u32 h, w, mines;
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

        void render();
        Helper::State *state;
        void renderBoard();
        
        void addMines();
        void countMines();
        bool isMine(u32 x, u32 y);
        enum TileState {OPEN, UNOPENNED, FLAGGED};

        std::vector<std::vector<std::pair<int, Game::TileState>>> board;
};

#endif