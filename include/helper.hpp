#ifndef HELPER_HPP
#define HELPER_HPP

#include <switch.h>
#include <stdint.h>

class Helper {
    public:
        static void renderImage(u32 x, u32 y, u32 h, u32 w, u8 *imgptr);
        static void drawCursor(u32 x, u32 y, u32 h, u32 w, u32 thicc, u32 color);
        static void drawRectangle(u32 x, u32 y, u32 h, u32 w, u32 color);
        enum State {menu_s, ten, forty, ninetynine, game_s};
};

#endif