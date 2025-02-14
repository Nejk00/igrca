#include"game.hpp"
#include "Map.hpp"
#include"ECS/ECS.hpp"
#include <iostream>
#include <ostream>

#include "Collision.hpp"
#include "ECS/ColiderComponent.hpp"
#include"ECS/Components.hpp"
#include "ECS/TileComponent.hpp"


Map* map;
SDL_Renderer* Game :: renderer = nullptr;
SDL_Event Game :: event;

Manager manager;

std::vector<ColiderComponent*> Game :: coliders;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());



Game :: Game() {

}
Game :: ~Game() {

}

void Game :: init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL_Init OK" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags );
        if (window) {
            std::cout << "SDL_CreateWindow OK" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "SDL_CreateRenderer OK" << std::endl;
        }
        isRunning = true;
    }
    else {
        isRunning = false;
    }

    map = new Map();

    tile0.addComponent<TileComponent>(200, 200, 32, 32, 0);
    tile1.addComponent<TileComponent>(250, 250, 32, 32, 1);
    tile1.addComponent<ColiderComponent>("dirt");
    tile2.addComponent<TileComponent>(150, 150, 32, 32, 2);
    tile2.addComponent<ColiderComponent>("grass");

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/minilebron.png");
    player.addComponent<Keyboard>();
    player.addComponent<ColiderComponent>("player");

    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 15, 1);
    wall.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/dirt.png");
    wall.addComponent<ColiderComponent>("wall");
}




void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
        break;
        default:
            break;
    }
}

void Game :: update() {
    manager.refresh();
    manager.update();
    for (auto cc : coliders) {
        Collision :: AABB(player.getComponent<ColiderComponent>(), *cc);
    }


}


void Game :: render() {
    SDL_RenderClear(renderer);
    map->DrawMap();
    manager.draw();
    SDL_RenderPresent(renderer);

}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

