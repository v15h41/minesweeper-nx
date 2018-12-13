#ifndef MENU_HPP
#define MENU_HPP

#include <switch.h>
#include <stdint.h>
#include <stdio.h>
#include "helper.hpp"
#include <string>
extern const ffnt_header_t tahoma24_nxfnt;
#define tahoma24 &tahoma24_nxfnt

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