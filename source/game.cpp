#include "game.hpp"

#define BOMB -1

void Game::update() {
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & KEY_A) {
        if (Game::checkCursorExists()) {
            this->mines->attemptOpenBlock(this->cursor_pos.first, this->cursor_pos.second);
        }
    }

    if (kDown & KEY_B) {
        if (Game::checkCursorExists()) {
            this->mines->flagBlock(this->cursor_pos.first, this->cursor_pos.second);
        }
    }

    if (kDown & KEY_X) {
        *(this->state) = Helper::menu_s;
    }

    if (kDown & KEY_UP) {
        Game::moveCursor(0, -1);
    }

    if (kDown & KEY_LEFT) {
        Game::moveCursor(-1, 0);
    }

    if (kDown & KEY_RIGHT) {
        Game::moveCursor(1, 0);
    }
    
    if (kDown & KEY_DOWN) {
        Game::moveCursor(0, 1);
    }

    if (this->mines->getGameState() != Mines::RUNNING) {
        *(this->state) = Helper::menu_s;
    }
    

    Game::render();
}

Game::Game(u32 h, u32 w, u32 mines, Helper::State *state) {
    this->mines = new Mines(h, w, mines);
    
    this->h = h;
    this->w = w;
    this->state = state;
    *(this->state) = Helper::game_s;
    this->cursor_pos = std::make_pair(-1,-1);
}



bool Game::checkCursorExists() {
    if (this->cursor_pos.first == -1 || this->cursor_pos.second == -1) {
        this->cursor_pos.first = 0;
        this->cursor_pos.second = 0;

        return false;
    }

    return true;
}

void Game::moveCursor(int xdir, int ydir) {
    if (Game::checkCursorExists()) {
        if (this->cursor_pos.first == 0 && xdir == -1) {
            this->cursor_pos.first = this->w - 1; 
        } else if (this->cursor_pos.first == this->w - 1 && xdir == 1) {
            this->cursor_pos.first = 0;
        } else {
            cursor_pos.first += xdir;
        }

        if (this->cursor_pos.second == 0 && ydir == -1) {
            this->cursor_pos.second = this->h - 1;
        } else if (this->cursor_pos.second == this->h - 1 && ydir == 1) {
            this->cursor_pos.second = 0;
        } else {
            cursor_pos.second += ydir;
        }
    }
}


void Game::render() {
    //Helper::renderImage(10, 10, 164, 164, this->smiley_pic);
    u32 bg = RGBA8_MAXALPHA(255, 255, 255);
    Helper::drawRectangle(0,0,720,1280, bg);
    u32 color = RGBA8_MAXALPHA(255, 0, 0);
    //Helper::drawRectangle(320, 30, 640, 640, color);
    
    Game::renderBoard();
}



void Game::renderBoard() {
    u32 board_color = RGBA8_MAXALPHA(128, 128, 128);
    u32 y = (640-40*this->h)/2 + 30;
    u32 x = (1280-40*w)/2; 

    Helper::drawRectangle(x, y, 40*this->h, 40*this->w, board_color);

    u32 i, j;
    for (i = 0; i < this->h; i++) {
        for (j = 0; j < this->w; j++) {
            if (this->mines->getState(j, i) == Mines::UNOPENNED) {
                Helper::renderImage(y+1, x+1, 38, 38, this->blocked_tile);
            } else if (this->mines->getState(j, i) == Mines::FLAGGED) {
                Helper::renderImage(y+1, x+1, 38, 38, this->flagged_tile);
            } else {
                switch (this->mines->getValue(j, i)) {
                    case BOMB:
                        Helper::renderImage(y+1, x+1, 38, 38, this->mine_tile);
                        break;
                    case 0:
                        Helper::renderImage(y+1, x+1, 38, 38, this->empty_tile);
                        break;
                    case 1:
                        Helper::renderImage(y+1, x+1, 38, 38, this->one_tile);
                        break;
                    case 2:
                        Helper::renderImage(y+1, x+1, 38, 38, this->two_tile);
                        break;
                    case 3:
                        Helper::renderImage(y+1, x+1, 38, 38, this->three_tile);
                        break;
                    case 4:
                        Helper::renderImage(y+1, x+1, 38, 38, this->four_tile);
                        break;
                    case 5:
                        Helper::renderImage(y+1, x+1, 38, 38, this->five_tile);
                        break;
                    case 6:
                        Helper::renderImage(y+1, x+1, 38, 38, this->six_tile);
                        break;
                    case 7:
                        Helper::renderImage(y+1, x+1, 38, 38, this->seven_tile);
                        break;
                    case 8:
                        Helper::renderImage(y+1, x+1, 38, 38, this->eight_tile);
                        break;                    
                }
            }

            if (this->cursor_pos.first == j && this->cursor_pos.second == i) {
                u32 cur_color = RGBA8_MAXALPHA(0, 0, 0);
                Helper::drawCursor(x+1, y+1, 38, 38, 4, cur_color);
            }
            
            x += 40;
        }
        y += 40;
        x = (1280-40*w)/2; 
    }
}