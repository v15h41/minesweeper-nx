#ifndef HELPER_HPP
#define HELPER_HPP

#include <switch.h>
#include <stdint.h>
#include "font.hpp"

class Helper {
    public:
        static void renderImage(u32 x, u32 y, u32 h, u32 w, u8 *imgptr);
        static void drawCursor(u32 x, u32 y, u32 h, u32 w, u32 thicc, u32 color);
        static void drawRectangle(u32 x, u32 y, u32 h, u32 w, u32 color);
        static void drawText(const ffnt_header_t* font, int x, int y, u32 color, const char* str);
        enum State {menu_s, ten, forty, ninetynine, game_s};
};

#endif