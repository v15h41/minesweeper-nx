#include "game.hpp"
#include <stdio.h>

void Game::update() {
    Game::render();
}

Game::Game(u32 h, u32 w, u32 mines, Helper::State *state) {
    this->h = h;
    this->w = w;
    this->mines = mines;
    this->state = state;
    *(this->state) = Helper::game_s;
}

void Game::render() {
    //Helper::renderImage(10, 10, 164, 164, this->smiley_pic);
    u32 bg = RGBA8_MAXALPHA(0, 0, 0);
    Helper::drawRectangle(0,0,720,1280, bg);
    u32 color = RGBA8_MAXALPHA(255, 0, 0);
    Helper::drawRectangle(10, 10, 85, 250, color);
    

}