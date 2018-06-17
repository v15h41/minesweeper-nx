#include "game.hpp"

void Game::update() {
    Game::render();
}

void Game::render() {
    Helper::renderImage(10, 10, 164, 164, this->smiley_pic);
    u32 color = RGBA8_MAXALPHA(255, 0, 0);
    Helper::drawCursor(10, 10, 164, 164, 5, color);

}