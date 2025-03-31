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

bool Game :: inDungeon = false;

SDL_Renderer* Game :: renderer = nullptr;
SDL_Event Game :: event;

SDL_Rect Game :: camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager manager;

std::vector<ColiderComponent*> Game :: colliders;

std::vector<Entity*> bulletPool;
std::vector<Entity*> enemyPool;

int Map::sizeX = 60;
int Map::sizeY = 60;

std::string  mapFile = "assets/map_tiles.png";

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto&wall2(manager.addEntity());
auto& enemy(manager.addEntity());
auto& pet1(manager.addEntity());
auto& pet2(manager.addEntity());
auto& turret(manager.addEntity());
auto& lebron(manager.addEntity());
auto& lab(manager.addEntity());



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

        lab.addComponent<TransformComponent>(800, 450, 50, 50, 1);
        lab.addComponent<SpriteComponent>("assets/dirt.png");
        lab.addComponent<ColiderComponent>("lab");
        lab.addGroup(groupMap);

        //enemy.addComponent<TransformComponent>(500, 1000, 30, 22, 2, 1);
        //enemy.addComponent<SpriteComponent>("assets/enemy.png", true);
        //enemy.addComponent<ColiderComponent>("enemy");
        //enemy.addComponent<EnemyComponent>(&player);
        //enemy.addGroup(groupEnemies);
//
        //pet1.addComponent<TransformComponent>(300, 500, 32, 32, 1, 1);
        //pet1.addComponent<SpriteComponent>("assets/pet2.png", true);
        //pet1.addComponent<ColiderComponent>("pet");
        //pet1.addComponent<PetComponent>(&player);
        //pet1.addGroup(groupPet);
//
        //pet2.addComponent<TransformComponent>(400, 700, 32, 32, 1, 1);
        //pet2.addComponent<SpriteComponent>("assets/pet2.png", true);
        //pet2.addComponent<ColiderComponent>("pet2");
        //pet2.addComponent<PetComponent>(&player);
        //pet2.addGroup(groupPet);
//
//
        //turret.addComponent<TransformComponent>(450, 650, 32, 32, 1);
        //turret.addComponent<SpriteComponent>("assets/dirt.png");
        //turret.addComponent<ColiderComponent>("turret");
        //turret.addComponent<TurretComponent>(player);
        //turret.addGroup(groupTurrets);

        player.addComponent<TransformComponent>(256, 132, 28, 17, 2, 6);
        player.addComponent<SpriteComponent>("assets/player.png", true);
        player.addComponent<Keyboard>();
        player.addComponent<Mouse>();
        player.addComponent<ColiderComponent>("player");
        player.addGroup(groupPlayers);
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
    bullet.addComponent<TransformComponent>(x, y, 10, 10, 1, 1);
    bullet.addComponent<SpriteComponent>("assets/dirt.png");
    bullet.addComponent<ColiderComponent>("bullet");
    bullet.addComponent<BulletComponent>(mouseX, mouseY);
    bullet.addGroup(groupProjectile);
}

void Game :: changeMap() {
    mapFile = "assets/lab_tiles.png";
    tiles.clear();
    player.deactivate();
    lab.deactivate();
    Map::LoadMap("assets/lab.txt", Map::sizeX, Map::sizeY);
    player.addComponent<TransformComponent>(256, 132, 28, 17, 2, 6);
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

    turret.addComponent<TransformComponent>(450, 650, 32, 32, 1);
    turret.addComponent<SpriteComponent>("assets/dirt.png");
    turret.addComponent<ColiderComponent>("turret");
    turret.addComponent<TurretComponent>(player);
    turret.addGroup(groupTurrets);


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

    //turret.getComponent<TurretComponent>().posX = player.getComponent<TransformComponent>().position.x;
    //turret.getComponent<TurretComponent>().posY = player.getComponent<TransformComponent>().position.y;

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
            if (e->isActive() && p->isActive()) {
                if (Collision::AABB(p->getComponent<ColiderComponent>(), e->getComponent<ColiderComponent>())) {
                    p->deactivate();
                    bulletPool.push_back(p);
                    e->deactivate();
                    enemyPool.push_back(e);
                    break;
                }
            }
        }
    }
    for (auto& p : projectiles) {
        for (auto& t : tiles) {
            if (t->hasComponent<ColiderComponent>()) {
                if (Collision::AABB(p->getComponent<ColiderComponent>(), t->getComponent<ColiderComponent>())) {
                    p->deactivate();
                    bulletPool.push_back(p);
                }
            }
        }
    }

    if (Collision::AABB(player.getComponent<ColiderComponent>(), lab.getComponent<ColiderComponent>())) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F) {
            changeMap();
        }
    }


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
    for (auto& t : turrets) {
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

    SDL_RenderPresent(renderer);

}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

