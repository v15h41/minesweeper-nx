#include "game.hpp"

void Game::update() {
    Game::render();
}

void Game::render() {
    Helper::renderImage(50, 400, 164, 164, this->smiley_pic);
}