#include "game.hpp"
#include <sys/time.h>

#define BOMB -1

// update the game object based on inputs
void Game::update() {
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    
    touchPosition touch;
    u32 touchCount = hidTouchCount();

     // if the game is not running and a button is pressed, go back to the menu
    if (this->mines->getGameState() != Mines::RUNNING && (kDown || touchCount > 0)) {
        *(this->state) = Helper::menu_s;
    }

    // if touched, attempt to open the touched block
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

    // if A is pressed, either attempt to open a selected block or bring up the cursor
    if (kDown & KEY_A) {
        if (Game::checkCursorExists()) {
            Game::startGameTimer();
            this->mines->attemptOpenBlock(this->cursor_pos.first, this->cursor_pos.second);
        }
    }

    // if B is pressed, either flag a selected block or bring up the cursor 
    if (kDown & KEY_B) {
        if (Game::checkCursorExists()) {
            this->mines->flagBlock(this->cursor_pos.first, this->cursor_pos.second);
        }
    }

    // if X is pressed, go back to the menu
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
    
    Game::render();
}

// game object constructor
Game::Game(u32 h, u32 w, u32 mines, Helper::State *state) {
    // create a new Minesweeper game object
    this->mines = new Mines(h, w, mines);
    
    this->h = h;
    this->w = w;
    this->state = state;
    *(this->state) = Helper::game_s;
    this->cursor_pos = std::make_pair(-1,-1);
}

// begin the game timer
void Game::startGameTimer() {
    if (this->game_time == 0) {
        this->game_time = time(NULL);
    }
}

// map a pixel coordinate to a block on the board
std::pair<int, int> Game::pixelToPos(u32 x, u32 y) {
    u32 init_y = (640-40*this->h)/2 + 30;
    u32 init_x = (1280-40*w)/2; 

    int raw_y = y - init_y;
    int raw_x = x - init_x;

    // return -1 is the pixel is out of range or return a block coordinate
    if (raw_y < 0 || raw_x < 0 || raw_y > 40*this->h || raw_x > 40*this->w) {
        return std::make_pair(-1, -1);
    } else {
        return std::make_pair(raw_x/40, raw_y/40);
    }    
}

// get current time
long long Game::getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double difference = (tv.tv_usec);
    return difference;
}

// get time elapsed
long long Game::timeElapsed(long long time) {
    return Game::getCurrentTime() - time;
}

// check if the cursor exists
bool Game::checkCursorExists() {
    if (this->cursor_pos.first == -1 || this->cursor_pos.second == -1) {
        this->cursor_pos.first = 0;
        this->cursor_pos.second = 0;

        return false;
    }

    return true;
}

// move the cursor with wrap around
void Game::moveCursor(int xdir, int ydir) {
    if (Game::checkCursorExists()) {
        // if we are at the edge of x, wrap around otherwise move in the x dir
        if (this->cursor_pos.first == 0 && xdir == -1) {
            this->cursor_pos.first = this->w - 1; 
        } else if (this->cursor_pos.first == this->w - 1 && xdir == 1) {
            this->cursor_pos.first = 0;
        } else {
            cursor_pos.first += xdir;
        }

        // if we are at the edge of y, wrap around otherwise move in the y dir
        if (this->cursor_pos.second == 0 && ydir == -1) {
            this->cursor_pos.second = this->h - 1;
        } else if (this->cursor_pos.second == this->h - 1 && ydir == 1) {
            this->cursor_pos.second = 0;
        } else {
            cursor_pos.second += ydir;
        }
    }
}

// draw time text
void Game::renderTime() {
    std::string time_string;

    if (this->game_time == 0) {
        time_string = std::to_string(0);
    } else {
        time_string = std::to_string(Game::getCurrentTime());
    }

    Helper::drawText(tahoma24, 1, 680, RGBA8_MAXALPHA(0, 0, 0), time_string.c_str());
}

// render the game
void Game::render() {
    // draw the background
    u32 bg = RGBA8_MAXALPHA(255, 255, 255);
    Helper::drawRectangle(0,0,720,1280, bg);
    
    Game::renderBoard();
    Game::renderCursor();
    //Game::renderTime();    

    if (this->mines->getGameState() != Mines::RUNNING) {
        renderEnd();
    }
}


// render the cursor
void Game::renderCursor() {
    // render the cursor if it is visible
    if (this->cursor_pos.first != -1 && this->cursor_pos.second != -1) {
        // initial position of the cursor
        u32 y = (640-40*this->h)/2 + 30;
        u32 x = (1280-40*w)/2; 

        // change position based on block coordinate
        y += this->cursor_pos.second*40;
        x += this->cursor_pos.first*40;

        // draw a rectangle over the selected block
        u32 cur_color = RGBA8_MAXALPHA(0, 0, 0);
        Helper::drawCursor(x+1, y+1, 38, 38, 4, cur_color);
    }
}

void Game::renderEnd() {
    u32 color = RGBA8_MAXALPHA(0, 255, 255);
    Helper::drawRectangle(510, 310, 85, 250, color);

    if (this->mines->getGameState() == Mines::WON) {
        std::string res = "You Won!";
        Helper::drawText(tahoma24, 572, 330, RGBA8_MAXALPHA(0, 0, 0), res.c_str());
    } else {
        std::string res = "You Lost!";
        Helper::drawText(tahoma24, 572, 330, RGBA8_MAXALPHA(0, 0, 0), res.c_str());
    }   
}

// render the board
void Game::renderBoard() {
    // background board colour
    u32 board_color = RGBA8_MAXALPHA(128, 128, 128);
    // initial position of corner
    u32 y = (640-40*this->h)/2 + 30;
    u32 x = (1280-40*w)/2; 

    // draw board colour
    Helper::drawRectangle(x, y, 40*this->h, 40*this->w, board_color);

    // draw each block based on its state
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
        // reset x to initial position 
        x = (1280-40*w)/2; 
    }

}