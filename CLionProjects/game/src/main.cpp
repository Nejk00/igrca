#include <SDL.h>
#include <cstdlib>
#include "game.hpp"
#include"Clock.hpp"

unsigned int Clock :: last_tick_time = 0;
unsigned int Clock :: delta = 0;
Game *game = nullptr;

int main(int argumentCount, char * arguments[]){

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    game->init("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, false);

    while (game->running()) {
        Clock :: tick();

        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean();

    return 0;
}