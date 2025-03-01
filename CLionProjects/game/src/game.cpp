#include"game.hpp"
#include "Map.hpp"
#include"ECS/ECS.hpp"
#include <iostream>
#include <ostream>
#include"GameLogic.hpp"
#include"Collision.hpp"
#include<ctime>
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

SDL_Renderer* Game :: renderer = nullptr;
SDL_Event Game :: event;

SDL_Rect Game :: camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager manager;

std::vector<ColiderComponent*> Game :: colliders;

const char* mapFile = "C:/Users/nejcg/CLionProjects/game/assets/map_tiles.png";

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto&wall2(manager.addEntity());
auto& enemy(manager.addEntity());
auto& pet(manager.addEntity());



enum groupLables : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupPet,
};
auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& pets(manager.getGroup(groupPet));

Game :: Game() {

}
Game :: ~Game() {

}

void Game :: init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    srand(time(NULL));
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

    Map::LoadMap("C:/Users/nejcg/CLionProjects/game/assets/map.map", 80, 80);

    player.addComponent<TransformComponent>(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 32, 32, 2, 2);
    player.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/spritesheet.png", true);
    player.addComponent<Keyboard>();
    player.addComponent<ColiderComponent>("player");
    player.addGroup(groupPlayers);

    enemy.addComponent<TransformComponent>(rand()%1000,rand()%1000, 29, 22, 2, 1);
    enemy.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/enemy2.png", true);
    enemy.addComponent<ColiderComponent>("enemy");
    enemy.addGroup(groupEnemies);


    pet.addComponent<TransformComponent>(rand()%1000,rand()%1000, 32, 32, 1, 1);
    pet.addComponent<SpriteComponent>("C:/Users/nejcg/CLionProjects/game/assets/pet.png", true);
    pet.addComponent<ColiderComponent>("pet");
    pet.addGroup(groupPet);

    wall.addComponent<TransformComponent>(500.0f, 500.0f, 100, 60, 1);
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
void Game::CameraSystem() {
    camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH/2;
    camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT/2;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;//hvala
    if (camera.x + SCREEN_WIDTH > 80*32) camera.x = 80*32 - SCREEN_WIDTH;
    if (camera.y + SCREEN_HEIGHT > 80*32) camera.y = 80*32 - SCREEN_HEIGHT;
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
    for (auto& p : pets) {
        static bool follow = false;
        auto& petTransform = p->getComponent<TransformComponent>();
        auto& playerTransform = player.getComponent<TransformComponent>();

        // Calculate the direction vector
        float directionX = playerTransform.position.x - petTransform.position.x;
        float directionY = playerTransform.position.y - petTransform.position.y;

        // Normalize the direction vector
        float length = std::sqrt(directionX * directionX + directionY * directionY);
        if (length > 0) {
            directionX /= length;
            directionY /= length;
        }

        // Smoothly interpolate the velocity
        float lerpFactor = 0.05f;
        float targetVelocityX = directionX * petTransform.speed;
        float targetVelocityY = directionY * petTransform.speed;

        if (Collision::AABB(player.getComponent<ColiderComponent>(), pet.getComponent<ColiderComponent>())){
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F) {
                follow = !follow;
            }
            targetVelocityX = 0;
            targetVelocityY = 0;
        }

        if (follow)
        {
            petTransform.velocity.x += (targetVelocityX - petTransform.velocity.x) * lerpFactor;
            petTransform.velocity.y += (targetVelocityY - petTransform.velocity.y) * lerpFactor;
        }
        if (follow == false) {
            p->getComponent<SpriteComponent>().play("pet_idle");
        }
        else if (Collision::AABB(player.getComponent<ColiderComponent>(), pet.getComponent<ColiderComponent>())){
            p->getComponent<SpriteComponent>().play("pet_idle");
        }
        else if (directionX > 0){
            p->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
            p->getComponent<SpriteComponent>().play("pet_walk");
        }
        else{
            p->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
            p->getComponent<SpriteComponent>().play("pet_walk");
        }
    }

    for (auto& e : enemies) {
        int range = 200;
        float targetVelocityX;
        float targetVelocityY;
        auto& enemyTransform = e->getComponent<TransformComponent>();
        auto& playerTransform = player.getComponent<TransformComponent>();

        // Calculate the direction vector
        float directionX = playerTransform.position.x - enemyTransform.position.x;
        float directionY = playerTransform.position.y - enemyTransform.position.y;

        // Compute the distance
        float distance = std::sqrt(directionX * directionX + directionY * directionY);

        // Check if the enemy is within range
        if (distance <= range) {
            // Normalize the direction vector
            if (distance > 0) {
                directionX /= distance;
                directionY /= distance;
            }

            // Calculate target velocity
            targetVelocityX = directionX * enemyTransform.speed;
            targetVelocityY = directionY * enemyTransform.speed;
        }
        enemyTransform.velocity.x += (targetVelocityX - enemyTransform.velocity.x);
        enemyTransform.velocity.y += (targetVelocityY - enemyTransform.velocity.y);


        if (directionX > 0){
        e->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
        e->getComponent<SpriteComponent>().play("enemy_walk");
        }
        else{
        e->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
        e->getComponent<SpriteComponent>().play("enemy_walk");
        }
    }






    //GameLogic::petMovement(pet, player);
    //GameLogic::enemyMovement(enemy, player);

    CameraSystem();



    Collision::CheckCollisions(player, pet, colliders);
    Collision::CheckCollisions(pet, player, colliders);
    //Collision::CheckCollisions(player, tiles);
    //Collision::CheckCollisions(enemy, tiles);
}



void Game :: render() {
    SDL_RenderClear(renderer);
    for (auto& t : tiles) {
        if (t->getComponent<TransformComponent>().position.x < camera.x + SCREEN_WIDTH && t->getComponent<TransformComponent>().position.y < camera.y + SCREEN_HEIGHT )
        t->draw();
    }
    for (auto& pe : pets) {
        pe->draw();
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
