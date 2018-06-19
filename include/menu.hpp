#ifndef MENU_HPP
#define MENU_HPP

#include <switch.h>
#include <stdint.h>
#include <stdio.h>
#include "helper.hpp"

class Menu {
    public:
        Menu(Helper::State *state);
        void update();
    private:
        Helper::State *state;
        int cursor = -1;
        void render();
        void move_cursor(u32 dir);

};

#endif