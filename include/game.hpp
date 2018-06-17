#ifndef GAME_HPP
#define GAME_HPP

#include <switch.h>
#include <stdint.h>
#include "smiley_bin.h"
#include "cursor_bin.h"
#include "helper.hpp"

class Game {
    

    public:
        void update();
    private:
        u8* smiley_pic = (u8*)smiley_bin;
        u8* cursor_pic = (u8*)cursor_bin;
        void render();
};

#endif