#ifndef GAME_HPP
#define GAME_HPP

#include <switch.h>
#include <stdint.h>
#include "smiley_bin.h"
#include "helper.hpp"

class Game {
    u8* smiley_pic = (u8*)smiley_bin;
    public:
        void update();
    private:
        void render();
};

#endif