#include"game.hpp"
#include "Map.hpp"
#include"ECS/ECS.hpp"
#include <iostream>
#include <ostream>
#include"GameLogic.hpp"
#include"Collision.hpp"
#include<ctime>
#include "ECS/Components.hpp"
#include"Clock.hpp"
#include "ECS/TurretComponent.hpp"
const int Game :: SCREEN_WIDTH = 640;
const int Game :: SCREEN_HEIGHT = 640;

SDL_Renderer* Game :: renderer = nullptr;
SDL_Event Game :: event;

SDL_Rect Game :: camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager manager;

std::vector<ColiderComponent*> Game :: colliders;
std::vector<Entity*> toDestroy;

int Map::sizeX = 60;
int Map::sizeY = 60;

const char* mapFile = "assets/map_tiles.png";

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto&wall2(manager.addEntity());
auto& enemy(manager.addEntity());
auto& pet1(manager.addEntity());
auto& pet2(manager.addEntity());
auto& turret(manager.addEntity());
auto& lebron(manager.addEntity());



auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& pets(manager.getGroup(groupPet));
auto& projectiles(manager.getGroup(groupProjectile));
auto& turrets = manager.getGroup(groupTurrets);


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

    Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);

    player.addComponent<TransformComponent>(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 28, 17, 2, 6);
    player.addComponent<SpriteComponent>("assets/player.png", true);
    player.addComponent<Keyboard>();
    player.addComponent<Mouse>();
    player.addComponent<ColiderComponent>("player");
    player.addGroup(groupPlayers);

    enemy.addComponent<TransformComponent>(500, 1000, 30, 22, 2, 1);
    enemy.addComponent<SpriteComponent>("assets/enemy.png", true);
    enemy.addComponent<ColiderComponent>("enemy");
    enemy.addComponent<EnemyComponent>(&player);
    enemy.addGroup(groupEnemies);

    pet1.addComponent<TransformComponent>(300, 500, 32, 32, 1, 1);
    pet1.addComponent<SpriteComponent>("assets/pet2.png", true);
    pet1.addComponent<ColiderComponent>("pet");
    pet1.addComponent<PetComponent>(&player);
    pet1.addGroup(groupPet);

    pet2.addComponent<TransformComponent>(400, 700, 32, 32, 1, 1);
    pet2.addComponent<SpriteComponent>("assets/pet2.png", true);
    pet2.addComponent<ColiderComponent>("pet2");
    pet2.addComponent<PetComponent>(&player);
    pet2.addGroup(groupPet);


    turret.addComponent<TransformComponent>(700, 700, 32, 32, 1);
    turret.addComponent<SpriteComponent>("assets/dirt.png");
    turret.addComponent<ColiderComponent>("turret");
    turret.addComponent<TurretComponent>(player);
    turret.addGroup(groupTurrets);
    turret.addGroup(groupMap);

    lebron.addComponent<TransformComponent>(1100, 1700, 100, 100, 1);
    lebron.addComponent<SpriteComponent>("assets/lebron.png");
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
void Game :: addBullet(Entity* player, int mouseX, int mouseY) {
    int x = player->getComponent<TransformComponent>().position.x;
    int y = player->getComponent<TransformComponent>().position.y;
    auto& bullet(manager.addEntity());
    bullet.addComponent<TransformComponent>(x, y, 16, 16, 1, 1);
    bullet.addComponent<SpriteComponent>("assets/dirt.png");
    bullet.addComponent<ColiderComponent>("bullet");
    bullet.addComponent<BulletComponent>(mouseX, mouseY);
    bullet.addGroup(groupProjectile);
}
/*void changeMap() {
    Map::sizeX = 80;
    Map::sizeY = 80;
    Map::LoadMap("assets/map.map", Map::sizeX, Map::sizeY);
    player.getComponent<TransformComponent>().position.x = 100;
    player.getComponent<TransformComponent>().position.y = 100;

}*/
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
    std::cout<<player.getComponent<TransformComponent>().position.x<<" "<<player.getComponent<TransformComponent>().position.y<<std::endl;
    for (auto& pet : pets) {
        Collision::CheckCollisions(*pet, pets);
        if (Collision::AABB(player.getComponent<ColiderComponent>(), pet->getComponent<ColiderComponent>()) && (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F)) {
            pet->getComponent<PetComponent>().follow = !pet->getComponent<PetComponent>().follow;
        }
        if(Collision::AABB(player.getComponent<ColiderComponent>(), pet->getComponent<ColiderComponent>())) {
            pet->getComponent<TransformComponent>().velocity.x = 0;
            pet->getComponent<TransformComponent>().velocity.y = 0;
            pet->getComponent<SpriteComponent>().play("pet_idle");
        }
    }

    for (auto& p : projectiles) {
        for (auto& e : enemies) {
            if (Collision::AABB(p->getComponent<ColiderComponent>(), e->getComponent<ColiderComponent>())) {
                e->getComponent<TransformComponent>().position.x = -5000;
                e->getComponent<TransformComponent>().position.y = -5000;
                e->getComponent<EnemyComponent>().chase = false;
                //toDestroy.push_back(e);
                //toDestroy.push_back(p);
                break;
            }
        }
    }

    // Then destroy them
    //for (auto e : toDestroy) e->destroy();


    GameLogic::CameraSystem(player);

    Collision::CheckCollisions(player, tiles);
    Collision::CheckCollisions(enemy, tiles);
    Collision::CheckCollisions(pet1, tiles);
    Collision::CheckCollisions(pet2, tiles);

}


void Game :: render() {
    SDL_RenderClear(renderer);
    for (auto& t : tiles) {
        t->draw();
    }
    for (auto& p : pets) {
        p->draw();
    }
    for (auto& p : players) {
        p->draw();
    }
    for (auto& e : enemies) {
        e->draw();
    }
    for (auto& p : projectiles) {
        p->draw();
    }
    lebron.draw();
    wall.draw();
    SDL_RenderPresent(renderer);

}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

