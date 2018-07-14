#include "mines.hpp"
#define BOMB -1

Mines::Mines(u32 h, u32 w, u32 mines) {
    this->h = h;
    this->w = w;
    this->mines = mines;
    this->flags = 0;
    this->openned = 0;
    this->state = Mines::RUNNING;

    this->board =  std::vector<std::vector<std::pair<int, Mines::TileState>>>(h, std::vector<std::pair<int, Mines::TileState>>(w));

    Mines::addMines();
    Mines::countMines();
}

void Mines::attemptOpenBlock(u32 x, u32 y) {
    if (this->board[y][x].second == Mines::OPEN) {
        Mines::flagRemaining(x, y);
        Mines::checkNumberSatisfied(x, y);
    } else {
        Mines::openBlock(x, y);
    }
}

int Mines::getValue(u32 x, u32 y) {
    return this->board[y][x].first;
}

Mines::TileState Mines::getState(u32 x, u32 y) {
    return this->board[y][x].second;
}

void Mines::checkWin() {
    if (this->h*this->w - this->openned == this->mines) {
        this->state = Mines::WON;
    }
}

Mines::GameState Mines::getGameState() {
    return this->state;
}

void Mines::flagRemaining(u32 x, u32 y) {
    int blocks = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!Mines::outOfBounds(x+i, y+j) && (this->board[y+j][x+i].second == Mines::UNOPENNED || this->board[y+j][x+i].second == Mines::FLAGGED)) {
                blocks++;
            }
        }
    }

    if (blocks == this->board[y][x].first) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (!Mines::outOfBounds(x+i, y+j) && this->board[y+j][x+i].second == Mines::UNOPENNED) {
                    Mines::flagBlock(x+i, y+j);
                }
            }
        }
    }
}

void Mines::addMines() {
    u32 i, j;
    for (i = 0; i < this->h; i++) {
        for (j = 0; j < this->w; j++) {
            this->board[i][j] = std::make_pair(0, Mines::UNOPENNED);
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

void Mines::openBlock(u32 x, u32 y) {
    if (!Mines::outOfBounds(x, y) && this->board[y][x].second != Mines::FLAGGED 
        && this->board[y][x].second != Mines::OPEN) {
        if (this->board[y][x].first == BOMB) {
            this->state = Mines::LOST;
        } else {
            this->board[y][x].second = Mines::OPEN;
            this->openned++;
            Mines::checkWin();

            if (this->board[y][x].first == 0) {
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        Mines::openBlock(x+i, y+j);
                    }
                }
            }         
        }
    }    
}

void Mines::flagBlock(u32 x, u32 y) {
    if (this->board[y][x].second == Mines::UNOPENNED) {
        this->flags++;
        this->board[y][x].second = Mines::FLAGGED;
    } else if (this->board[y][x].second == Mines::FLAGGED) {
        this->flags--;
        this->board[y][x].second = Mines::UNOPENNED;
    }
}

void Mines::checkNumberSatisfied(u32 x, u32 y) {
    int flags = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!Mines::outOfBounds(x+i, y+j) && this->board[y+j][x+i].second == Mines::FLAGGED) {
                flags++;
            }
        }
    }

    if (flags == this->board[y][x].first) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                Mines::openBlock(x+i, y+j);
            }
        }
    }
}

bool Mines::outOfBounds(int x, int y) {
    if (x >= this->w || x < 0 || y >= this->h || y < 0) {
        return true;
    }
    return false;
}

bool Mines::isMine(u32 x, u32 y) {
    if (Mines::outOfBounds(x, y)) {
        return false;
    } else if (this->board[y][x].first == BOMB) {
        return true;
    } else {
        return false;
    }
}

void Mines::countMines() {
    u32 i, j;
    for (i = 0; i < this->h; i++) {
        for (j = 0; j < this->w; j++) {
            if (board[i][j].first != BOMB) {                
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        board[i][j].first += isMine(j+k, i+l) ? 1 : 0;
                    }
                }               
            }            
        }
    }
}