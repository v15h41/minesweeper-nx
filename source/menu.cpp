#include "menu.hpp"

void Menu::update() {
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & KEY_DOWN) {
        Menu::move_cursor(1);        
    }

    if (kDown & KEY_UP) {
        Menu::move_cursor(-1);
    }

    if (kDown & KEY_A) {
        if (this->cursor == -1) {
            this->cursor = 1;
        } else {
            *(this->state) = static_cast<Helper::State>(cursor);
        }
    }

    Menu::render();
}

Menu::Menu(Helper::State *state) {
    this->state = state;
}

void Menu::move_cursor(u32 dir) {
    if (this->cursor == -1) {
        this->cursor = 1;
    } else if (this->cursor != 3 && dir == 1) {
        this->cursor++;
    } else if (this->cursor != 1 && dir == -1) {
        this -> cursor--;
    }
}

void Menu::render() {
    u32 bg = RGBA8_MAXALPHA(0, 255, 255);
    Helper::drawRectangle(0,0,720,1280, bg);
    u32 color = RGBA8_MAXALPHA(172, 172, 172);
    Helper::drawRectangle(500, 175, 85, 250, color);
    Helper::drawRectangle(500, 310, 85, 250, color);
    Helper::drawRectangle(500, 445, 85, 250, color);

    if (this->cursor != -1) {
        u32 cur_color = RGBA8_MAXALPHA(0, 0, 0);
        switch(this->cursor) {
            case 1:
                Helper::drawCursor(500, 175, 85, 250, 5, cur_color);
                break;
            case 2:
                Helper::drawCursor(500, 310, 85, 250, 5, cur_color);
                break;
            case 3:
                Helper::drawCursor(500, 445, 85, 250, 5, cur_color);
                break;
        }
    }
}