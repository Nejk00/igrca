#include"game.hpp"
#include "Map.hpp"
#include"ECS/ECS.hpp"
#include <iostream>
#include <ostream>

#include "Collision.hpp"
#include "ECS/ColiderComponent.hpp"
#include"ECS/Components.hpp"
#include "ECS/TileComponent.hpp"
#include "ECS/TrackComponent.hpp"
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

SDL_Renderer* Game :: renderer = nullptr;
SDL_Event Game :: event;

SDL_Rect Game :: camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager manager;

std::vector<ColiderComponent*> Game :: coliders;

const char* mapFile = "C:/Users/nejcg/CLionProjects/game/assets/map_tiles.png";

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto&wall2(manager.addEntity());
auto& enemy(manager.addEntity());


enum groupLables : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
};
auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

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

    Map::LoadMap("C:/Users/nejcg/CLionProjects/game/assets/map.map", 16, 16);

    player.addComponent<TransformComponent>(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 28, 17, 2);
    player.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/player.png", true);
    player.addComponent<Keyboard>();
    player.addComponent<ColiderComponent>("player");


    player.addGroup(groupPlayers);

    enemy.addComponent<TransformComponent>(500, 20, 32, 32, 1);
    enemy.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/dirt.png");
    enemy.addComponent<ColiderComponent>("enemy");
    //enemy.addComponent<TrackComponent>(5);
    enemy.addGroup(groupMap);

    wall.addComponent<TransformComponent>(500.0f, 500.0f, 10, 50, 3);
    wall.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/wall.png");
    wall.addComponent<ColiderComponent>("wall");
    wall.addGroup(groupMap);


}


void Game::AddTile(int srcX, int srcY, int xpos, int ypos, bool hasCollision) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapFile);
    tile.addComponent<TransformComponent>(xpos, ypos, 32, 32, 1);
    tile.addGroup(groupMap);
    if (hasCollision) {
        tile.addComponent<ColiderComponent>("tile_" + std::to_string(xpos) + "_" + std::to_string(ypos));
    }
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

    camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH/2;
    camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT/2;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > SCREEN_WIDTH) camera.x = SCREEN_WIDTH;
    if (camera.y > SCREEN_HEIGHT) camera.y = SCREEN_HEIGHT;
    /*Vector2D pVel = player.getComponent<TransformComponent>().velocity;
    int pSpeed = player.getComponent<TransformComponent>().speed;

    // Calculate the offset for this frame
    Vector2D offset = { -(pVel.x * pSpeed), -(pVel.y * pSpeed) };

    for (auto& t : tiles) {
        if (t && t->hasComponent<TileComponent>()) {
            auto& tileComponent = t->getComponent<TileComponent>();

            // Apply the offset to the tile's position
            tileComponent.destRect.x += offset.x;
            tileComponent.destRect.y += offset.y;
        }
    }*/
    for (auto& cc : coliders) {
        auto& playerTransform = player.getComponent<TransformComponent>();
        auto& playerCollider = player.getComponent<ColiderComponent>().collider;

        // Predict next position
        float nextX = playerTransform.position.x + playerTransform.velocity.x * playerTransform.speed;
        float nextY = playerTransform.position.y + playerTransform.velocity.y * playerTransform.speed;

        SDL_Rect nextPlayerCollider = playerCollider;
        nextPlayerCollider.x = static_cast<int>(nextX);
        nextPlayerCollider.y = static_cast<int>(nextY);

        // Check collision with all colliders
        for (auto& otherColliderComponent : coliders) {
            if (otherColliderComponent == &player.getComponent<ColiderComponent>()) {
                continue; // Skip the player's own collider
            }

            auto& otherCollider = otherColliderComponent->collider;

            if (SDL_HasIntersection(&nextPlayerCollider, &otherCollider)) {
                // Separate horizontal and vertical checks
                SDL_Rect horizontalTest = playerCollider;
                horizontalTest.x = static_cast<int>(nextX);

                SDL_Rect verticalTest = playerCollider;
                verticalTest.y = static_cast<int>(nextY);

                if (SDL_HasIntersection(&horizontalTest, &otherCollider))
                    playerTransform.velocity.x = 0; // Stop horizontal movement

                if (SDL_HasIntersection(&verticalTest, &otherCollider))
                    playerTransform.velocity.y = 0; // Stop vertical movement

            }
        }
    }
}


void Game :: render() {
    SDL_RenderClear(renderer);
    for (auto& t : tiles) {
        t->draw();
    }
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
