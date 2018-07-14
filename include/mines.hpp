#ifndef MINES_HPP
#define MINES_HPP

#include <switch.h>
#include <stdint.h>
#include "helper.hpp"
#include <vector>
#include <utility>
#include <stdlib.h> 
#include <ctime>
#include <stdio.h>

class Mines {
    public:
        enum TileState {OPEN, UNOPENNED, FLAGGED};
        enum GameState {WON, LOST, RUNNING};

        void attemptOpenBlock(u32 x, u32 y);
        void flagBlock(u32 x, u32 y);
        int getValue(u32 x, u32 y);
        TileState getState(u32 x, u32 y);
        int checkEnd();
        GameState getGameState();
        Mines(u32, u32, u32);
    private:
        u32 h, w, mines, flags, openned;
        
        GameState state; 
        std::vector<std::vector<std::pair<int, Mines::TileState>>> board;

        void openBlock(u32 x, u32 y);
        void addMines();
        void countMines();
        bool isMine(u32 x, u32 y);   
        bool outOfBounds(int x, int y);        
        void checkNumberSatisfied(u32 x, u32 y);
        void checkWin();
        void flagRemaining(u32 x, u32 y);
};



#endif