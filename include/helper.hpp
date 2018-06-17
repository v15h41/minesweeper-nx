#ifndef HELPER_HPP
#define HELPER_HPP

#include <switch.h>
#include <stdint.h>

class Helper {
    public:
        static void renderImage(u32 x, u32 y, u32 h, u32 w, u8 *imgptr);
};

#endif