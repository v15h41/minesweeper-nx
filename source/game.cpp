#include "game.hpp"

#define BOMB -1

void Game::update() {
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    
    touchPosition touch;
    u32 touchCount = hidTouchCount();

    if (touchCount > 0) {
        hidTouchRead(&touch, 0);
        std::pair<int, int> coords = Game::pixelToPos(touch.px, touch.py);

        if (coords.first != -1) {
            Game::startGameTimer();
            this->cursor_pos.first = -1;
            this->cursor_pos.second = -1;

            this->mines->attemptOpenBlock(coords.first, coords.second);
        }
    }

    if (kDown & KEY_A) {
        if (Game::checkCursorExists()) {
            Game::startGameTimer();
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

void Game::startGameTimer() {
    if (this->game_time == 0) {
        this->game_time = time(NULL);
    }
}

std::pair<int, int> Game::pixelToPos(u32 x, u32 y) {
    u32 init_y = (640-40*this->h)/2 + 30;
    u32 init_x = (1280-40*w)/2; 

    int raw_y = y - init_y;
    int raw_x = x - init_x;

    if (raw_y < 0 || raw_x < 0 || raw_y > 40*this->h || raw_x > 40*this->w) {
        return std::make_pair(-1, -1);
    } else {
        return std::make_pair(raw_x/40, raw_y/40);
    }    
}

u32 Game::timeElapsed(u32 time) {
    time_t unixTime = std::time(NULL);

    return unixTime - time;
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

void Game::renderTime() {
    std::string time_string;

    if (this->game_time == 0) {
        time_string = std::to_string(0);
    } else {
        time_string = std::to_string(Game::timeElapsed(this->game_time));
    }

    Helper::drawText(tahoma24, 1, 680, RGBA8_MAXALPHA(0, 0, 0), time_string.c_str());
}


void Game::render() {
    //Helper::renderImage(10, 10, 164, 164, this->smiley_pic);
    u32 bg = RGBA8_MAXALPHA(255, 255, 255);
    Helper::drawRectangle(0,0,720,1280, bg);
    u32 color = RGBA8_MAXALPHA(255, 0, 0);
    //Helper::drawRectangle(320, 30, 640, 640, color);
    
    Game::renderBoard();
    Game::renderCursor();
    //std::string coord_string = std::to_string(this->touchx) + " " + std::to_string(this->touchy);
    //const char* coord_s = coord_string.c_str();
    //Helper::drawText(tahoma24, 1, 680, RGBA8_MAXALPHA(0, 0, 0), coord_s);
    Game::renderTime();
    
}



void Game::renderCursor() {
    if (this->cursor_pos.first != -1 && this->cursor_pos.second != -1) {
        u32 y = (640-40*this->h)/2 + 30;
        u32 x = (1280-40*w)/2; 

        y += this->cursor_pos.second*40;
        x += this->cursor_pos.first*40;

        u32 cur_color = RGBA8_MAXALPHA(0, 0, 0);
        Helper::drawCursor(x+1, y+1, 38, 38, 4, cur_color);
    }
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

            x += 40;
        }
        y += 40;
        x = (1280-40*w)/2; 
    }
}