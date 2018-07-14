#include "helper.hpp"

void Helper::renderImage(u32 x, u32 y, u32 h, u32 w, u8 *imgptr) {
    u32 width, height;
    u32 *framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);
    
    u32 i, j;
    for (i =0; i < h; i++) {
        for (j = 0; j < w; j++) {
            u32 pos = (y+i) + (x+j)*width;
            framebuf[pos] = RGBA8_MAXALPHA(imgptr[(j*w+i)*4+0], imgptr[(j*w+i)*4+1], imgptr[(j*w+i)*4+2]);
        }
    }
}

void Helper::drawCursor(u32 x, u32 y, u32 h, u32 w, u32 thicc, u32 color) {
    Helper::drawRectangle(x-thicc, y-thicc, thicc, w+2*thicc, color);
    Helper::drawRectangle(x-thicc, y-thicc, h+2*thicc, thicc, color);
    Helper::drawRectangle(x+w, y-thicc, h+2*thicc, thicc, color);
    Helper::drawRectangle(x-thicc, y+h, thicc, w+2*thicc, color);
}

void Helper::drawRectangle(u32 x, u32 y, u32 h, u32 w, u32 color) {
    u32 width, height;
    u32 *framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);

    u32 i, j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            u32 pos = (y+i)*width + (x+j);
            framebuf[pos] = color;
        }
    }
}

void Helper::drawText(const ffnt_header_t* font, int x, int y, u32 color, const char* str) {
    color_t clr;
    clr.abgr = color;
    DrawText(font, x, y, clr, str);
}