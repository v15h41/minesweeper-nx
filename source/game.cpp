#include "game.hpp"

#define BOMB -1

void Game::update() {
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & KEY_A) {
        *(this->state) = Helper::menu_s;
    }

    Game::render();
}

Game::Game(u32 h, u32 w, u32 mines, Helper::State *state) {
    this->h = h;
    this->w = w;
    this->mines = mines;
    this->state = state;
    *(this->state) = Helper::game_s;

    this->board =  std::vector<std::vector<std::pair<int, Game::TileState>>>(h, std::vector<std::pair<int, Game::TileState>>(w));

    Game::addMines();
    Game::countMines();
}

void Game::addMines() {
    u32 i, j;
    for (i = 0; i < this->h; i++) {
        for (j = 0; j < this->w; j++) {
            this->board[i][j] = std::make_pair(0, Game::UNOPENNED);
        }
    }
    
    srand(time(NULL));    
    for (i = 0; i < this->mines; i++) {
        int x, y;

        do {
            
            x = rand() % this->w;
            y = rand() % this->h;
            
        } 
        while (this->board[y][x].first == BOMB);

        this->board[y][x].first = BOMB;
    }
}

bool Game::isMine(u32 x, u32 y) {
    if (x >= this->w || x < 0 || y >= this->h || y < 0) {
        return false;
    } else if (this->board[y][x].first == BOMB) {
        return true;
    } else {
        return false;
    }
}

void Game::countMines() {
    u32 i, j;
    for (i = 0; i < this->h; i++) {
        for (j = 0; j < this->w; j++) {
            if (board[i][j].first != BOMB) {
                board[i][j].first += isMine(j+1, i) ? 1 : 0;
                board[i][j].first += isMine(j-1, i) ? 1 : 0;
                board[i][j].first += isMine(j, i+1) ? 1 : 0;
                board[i][j].first += isMine(j, i-1) ? 1 : 0;
                board[i][j].first += isMine(j+1, i+1) ? 1 : 0;
                board[i][j].first += isMine(j-1, i-1) ? 1 : 0;
                board[i][j].first += isMine(j-1, i+1) ? 1 : 0;
                board[i][j].first += isMine(j+1, i-1) ? 1 : 0;
            }            
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
            if (this->board[i][j].second == Game::UNOPENNED) {
                Helper::renderImage(y+1, x+1, 38, 38, this->blocked_tile);
            } else if (this->board[i][j].second == Game::FLAGGED) {
                Helper::renderImage(y+1, x+1, 38, 38, this->flagged_tile);
            } else {
                switch (this->board[i][j].first) {
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