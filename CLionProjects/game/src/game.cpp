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
auto& enemy(manager.addEntity());

enum groupLables : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
};

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
    //Map::LoadMap("C:/Users/nejcg/CLionProjects/game/assets/map.txt", 20, 20);

    player.addComponent<TransformComponent>(220, 320, 28, 17, 2);
    player.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/player.png", true);
    player.addComponent<Keyboard>();
    player.addComponent<ColiderComponent>("player");
    player.addGroup(groupPlayers);

    /*enemy.addComponent<TransformComponent>(300.0f, 100.0f, 100, 100, 2);
    enemy.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/LEBRONNNN.png");
    enemy.addComponent<ColiderComponent>("enemy");
    enemy.addGroup(groupEnemies);*/

    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 200, 1);
    wall.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/dirt.png");
    wall.addComponent<ColiderComponent>("wall");
    wall.addGroup(groupEnemies);
}


void Game::AddTile(int id, int x, int y) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 32, 32, id);
    tile.addGroup(groupMap);
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

    /*for (auto cc : coliders){
        if (Collision :: AABB(player.getComponent<ColiderComponent>(), *cc))*/
    // Collision for walls
    auto& playerTransform = player.getComponent<TransformComponent>();
    auto& playerCollider = player.getComponent<ColiderComponent>().collider;

    // Predict next position (top-left origin correction)
    float nextX = playerTransform.position.x + playerTransform.velocity.x * playerTransform.speed;
    float nextY = playerTransform.position.y + playerTransform.velocity.y * playerTransform.speed;

    SDL_Rect nextPlayerCollider = playerCollider;
    nextPlayerCollider.x = static_cast<int>(nextX);
    nextPlayerCollider.y = static_cast<int>(nextY);

        if (SDL_HasIntersection(&nextPlayerCollider, &wall.getComponent<ColiderComponent>().collider)) {
            // Separate horizontal and vertical checks
            SDL_Rect horizontalTest = playerCollider;
            horizontalTest.x = static_cast<int>(nextX);

            SDL_Rect verticalTest = playerCollider;
            verticalTest.y = static_cast<int>(nextY);

            if (SDL_HasIntersection(&horizontalTest, &wall.getComponent<ColiderComponent>().collider)) {
                playerTransform.velocity.x = 0;
            }
            if (SDL_HasIntersection(&verticalTest, &wall.getComponent<ColiderComponent>().collider)) {
                playerTransform.velocity.y = 0;
            }
        }
    }
auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game :: render() {
    SDL_RenderClear(renderer);
    map->DrawMap();
    /*for (auto& t : tiles) {
        t->draw();
    }*/
    for (auto& p : players) {
        p->draw();
    }
    for (auto& e : enemies) {
        e->draw();
    }

    //manager.draw();

    SDL_RenderPresent(renderer);

}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
