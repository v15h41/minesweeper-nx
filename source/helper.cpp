#include "helper.hpp"

void Helper::renderImage(u32 x, u32 y, u32 h, u32 w, u8 *imgptr) {
    u32 width, height;
    u32 *framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);
    
    u32 i, j;
    for (i =0; i < w; i++) {
        for (j = 0; j < h; j++) {
            u32 pos = (y+i) * width + (x+j);
            framebuf[pos] = RGBA8_MAXALPHA(imgptr[(j+i*w)*4+0], imgptr[(j+i*w)*4+1], imgptr[(j+i*w)*4+2]);
        }
    }
}