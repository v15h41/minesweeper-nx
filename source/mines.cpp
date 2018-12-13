#include "mines.hpp"
#define BOMB -1

// initialise the minesweeper instance
Mines::Mines(u32 h, u32 w, u32 mines) {
    this->h = h;
    this->w = w;
    this->mines = mines;
    this->flags = 0;
    this->openned = 0;
    this->state = Mines::RUNNING;

    this->board =  std::vector<std::vector<std::pair<int, Mines::TileState>>>(h, std::vector<std::pair<int, Mines::TileState>>(w));

    // randomize mine locations and populate board with numbers
    Mines::addMines();
    Mines::countMines();
}

// attempt to open a block
void Mines::attemptOpenBlock(u32 x, u32 y) {
    // if a block is already open on a number
    if (this->board[y][x].second == Mines::OPEN) {
        Mines::flagRemaining(x, y);
        Mines::checkNumberSatisfied(x, y);
    // otherwise open the block
    } else {
        Mines::openBlock(x, y);
    }
}

// get value of square
int Mines::getValue(u32 x, u32 y) {
    return this->board[y][x].first;
}

// get state of square
Mines::TileState Mines::getState(u32 x, u32 y) {
    return this->board[y][x].second;
}

// check if all blocks except mines have been openned 
void Mines::checkWin() {
    if (this->h*this->w - this->openned == this->mines) {
        this->state = Mines::WON;
    }
}

// get the state of game
Mines::GameState Mines::getGameState() {
    return this->state;
}

// if the number of surrounding unopenned tiles are the same as the value of the 
// tile, flag all unopenned tiles
void Mines::flagRemaining(u32 x, u32 y) {
    int blocks = 0;

    // count blocks
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!Mines::outOfBounds(x+i, y+j) && (this->board[y+j][x+i].second == Mines::UNOPENNED || this->board[y+j][x+i].second == Mines::FLAGGED)) {
                blocks++;
            }
        }
    }

    // flag unopenned blocks
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

// randomly generate mines on the board
void Mines::addMines() {
    // init the board with unopenned blocks
    u32 i, j;
    for (i = 0; i < this->h; i++) {
        for (j = 0; j < this->w; j++) {
            this->board[i][j] = std::make_pair(0, Mines::UNOPENNED);
        }
    }
    
    // for each mine, attempt to place it in a random position on the board
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

// open a block 
void Mines::openBlock(u32 x, u32 y) {
    // only open if the block is within bounds and not flagged or open
    if (!Mines::outOfBounds(x, y) && this->board[y][x].second != Mines::FLAGGED 
        && this->board[y][x].second != Mines::OPEN) {
        // if the block is a bomb, you lose
        if (this->board[y][x].first == BOMB) {
            this->state = Mines::LOST;
        // otherwise open the block and check if we have won
        } else {            
            this->board[y][x].second = Mines::OPEN;
            this->openned++;
            Mines::checkWin();

            // open surrounding blocks if there are no mines touching the current one
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

// toggle the flagging of a block
void Mines::flagBlock(u32 x, u32 y) {
    if (this->board[y][x].second == Mines::UNOPENNED) {
        this->flags++;
        this->board[y][x].second = Mines::FLAGGED;
    } else if (this->board[y][x].second == Mines::FLAGGED) {
        this->flags--;
        this->board[y][x].second = Mines::UNOPENNED;
    }
}

// if the number of flags surrounding a block satisfy its value, then open all 
// other non-flagged blocked
void Mines::checkNumberSatisfied(u32 x, u32 y) {
    int flags = 0;

    // count the number of flags surrounding a block
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!Mines::outOfBounds(x+i, y+j) && this->board[y+j][x+i].second == Mines::FLAGGED) {
                flags++;
            }
        }
    }

    // if the number of flags match the value of the tile, open surrounding blocks
    if (flags == this->board[y][x].first) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                Mines::openBlock(x+i, y+j);
            }
        }
    }
}

// check if a coordinate is out of bounds
bool Mines::outOfBounds(int x, int y) {
    if (x >= this->w || x < 0 || y >= this->h || y < 0) {
        return true;
    }
    return false;
}

// check if a block is out of bounds or is a mine
bool Mines::isMine(u32 x, u32 y) {
    if (Mines::outOfBounds(x, y)) {
        return false;
    } else if (this->board[y][x].first == BOMB) {
        return true;
    } else {
        return false;
    }
}

// count the number of mines that surround a non-mine block
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