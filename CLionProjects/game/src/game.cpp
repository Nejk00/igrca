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
int Game :: all_pets = -1;

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
auto& enemy(manager.addEntity());
auto& enemy2(manager.addEntity());
auto& pet1(manager.addEntity());
auto& pet2(manager.addEntity());
auto& pet3(manager.addEntity());
auto& pet4(manager.addEntity());
auto& pet5(manager.addEntity());
auto& pet6(manager.addEntity());
auto& turret(manager.addEntity());
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

        window = SDL_CreateWindow(title, xpos, ypos, SCREEN_WIDTH, SCREEN_HEIGHT, flags );
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
        bulletTexture = Texture::LoadTexture("assets/player_bullet.png");
        hpTexture = Texture::LoadTexture("assets/heart.png");
        game_overTexture = Texture::LoadTexture("assets/game_over.png");
        winTexture = Texture::LoadTexture("assets/win.png");
        savedPet = Texture::LoadTexture("assets/pet.png");

        Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);

        lab.addComponent<TransformComponent>(800, 450, 16, 16, 2);
        lab.addComponent<SpriteComponent>("assets/lojtra.png");
        lab.addComponent<ColiderComponent>("lab");
        lab.addGroup(groupMap);

        player.addComponent<TransformComponent>(256, 132, 28, 17, 2, 3);
        player.addComponent<SpriteComponent>("assets/player.png", true);
        player.addComponent<Keyboard>();
        player.addComponent<ColiderComponent>("player");
        player.addComponent<HPComponent>(3);
        player.addComponent<PlayerComponent>();
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
void Game :: addBullet(Entity* object, int targetX, int targetY) {
    int x = object->getComponent<TransformComponent>().position.x + object->getComponent<TransformComponent>().width/2;
    int y = object->getComponent<TransformComponent>().position.y + object->getComponent<TransformComponent>().height/2;

    auto& bullet(manager.addEntity());

    bullet.addComponent<TransformComponent>(x, y, 14, 14, 1, 1);
    bullet.addComponent<ColiderComponent>("bullet");
    bullet.addComponent<BulletComponent>(targetX, targetY);
    bullet.addGroup(groupProjectile);

    if (object->getComponent<ColiderComponent>().tag == "player") {
        bullet.addComponent<SpriteComponent>("assets/player_bullet.png", true);
    }
    else {
        bullet.addComponent<SpriteComponent>("assets/bullet.png", true);
    }
}

void Game :: changeMap(float prevX, float prevY) {
    if (inDungeon) {
        mapFile = "assets/lab_tiles.png";
        tiles.clear();
        Map::LoadMap("assets/lab.txt", Map::sizeX, Map::sizeY);

        player.getComponent<TransformComponent>().position.x = 256;
        player.getComponent<TransformComponent>().position.y = 132;

        lab.getComponent<TransformComponent>().position.x = 1570;
        lab.getComponent<TransformComponent>().position.y = 1650;
        lab.addGroup(groupMap);

        enemy.addComponent<TransformComponent>(450, 600, 30, 22, 2, 1);
        enemy.addComponent<SpriteComponent>("assets/enemy.png", true);
        enemy.addComponent<ColiderComponent>("enemy");
        enemy.addComponent<EnemyComponent>(&player, "enemy");
        enemy.addGroup(groupEnemies);

        enemy2.addComponent<TransformComponent>(1500, 300, 41, 62, 1, 1);
        enemy2.addComponent<SpriteComponent>("assets/wolf.png", true);
        enemy2.addComponent<ColiderComponent>("wolf");
        enemy2.addComponent<EnemyComponent>(&player, "wolf");
        enemy2.addGroup(groupEnemies);

        pet1.addComponent<TransformComponent>(380, 600, 32, 32, 1, 1);
        pet1.addComponent<SpriteComponent>("assets/pet.png", true);
        pet1.addComponent<ColiderComponent>("pet");
        pet1.addComponent<PetComponent>(&player);
        pet1.addGroup(groupPet);

        pet2.addComponent<TransformComponent>(1500, 260, 32, 32, 1, 1);
        pet2.addComponent<SpriteComponent>("assets/pet2.png", true);
        pet2.addComponent<ColiderComponent>("pet");
        pet2.addComponent<PetComponent>(&player);
        pet2.addGroup(groupPet);

        pet3.addComponent<TransformComponent>(950, 1510, 32, 32, 1, 1);
        pet3.addComponent<SpriteComponent>("assets/pet2.png", true);
        pet3.addComponent<ColiderComponent>("pet");
        pet3.addComponent<PetComponent>(&player);
        pet3.addGroup(groupPet);

        pet4.addComponent<TransformComponent>(1550, 820, 32, 32, 1, 1);
        pet4.addComponent<SpriteComponent>("assets/pet.png", true);
        pet4.addComponent<ColiderComponent>("pet");
        pet4.addComponent<PetComponent>(&player);
        pet4.addGroup(groupPet);

        pet5.addComponent<TransformComponent>(1550, 1560, 32, 32, 1, 1);
        pet5.addComponent<SpriteComponent>("assets/pet.png", true);
        pet5.addComponent<ColiderComponent>("pet");
        pet5.addComponent<PetComponent>(&player);
        pet5.addGroup(groupPet);

        pet6.addComponent<TransformComponent>(270, 1420, 32, 32, 1, 1);
        pet6.addComponent<SpriteComponent>("assets/pet.png", true);
        pet6.addComponent<ColiderComponent>("pet");
        pet6.addComponent<PetComponent>(&player);
        pet6.addGroup(groupPet);

        turret.addComponent<TransformComponent>(450, 650, 32, 32, 1);
        turret.addComponent<SpriteComponent>("assets/dirt.png");
        turret.addComponent<ColiderComponent>("turret");
        turret.addComponent<TurretComponent>(&player);
        turret.addGroup(groupTurrets);
    }
    else if (!inDungeon) {
        mapFile = "assets/map_tiles.png";
        tiles.clear();
        enemies.clear();
        pets.clear();
        turrets.clear();
        Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);

        player.getComponent<TransformComponent>().position.x = prevX;
        player.getComponent<TransformComponent>().position.y = prevY;

        lab.getComponent<TransformComponent>().position.x = prevX;
        lab.getComponent<TransformComponent>().position.y = prevY;
        lab.addGroup(groupMap);
    }


}
void Game::handleEvents() {
    bool clicked = false;
    static int max_ammo = 5;
    static int ammo_reload = 0;

    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
             isRunning = false;
        break;
        default:
            break;
    }
    if (inDungeon) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                clicked = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                clicked = false;
            }
        }
    }
    if (player.getComponent<PlayerComponent>().ammo_count > 0 && clicked) {
        Game::addBullet(&player, event.button.x, event.button.y);
        player.getComponent<PlayerComponent>().ammo_count--;
    }
    if (player.getComponent<PlayerComponent>().ammo_count == max_ammo) {
        ammo_reload = 0;
    }
    else if (ammo_reload > 7000){
        player.getComponent<PlayerComponent>().ammo_count++;
        ammo_reload = 0;
    }
    ammo_reload += Clock :: delta;
}

void Game :: update() {
    manager.refresh();
    manager.update();

    if (pets.size() > 0) {
        all_pets = pets.size();
    }

    for (auto& e : enemies) {
        if (e->isActive() && e->hasComponent<ColiderComponent>()) {
            if (Collision::blockVision(e->getComponent<ColiderComponent>().collider, player.getComponent<ColiderComponent>().collider, tiles)) {
                if (e->getComponent<EnemyComponent>().sinceLastSeen == 0) {
                }
                e->getComponent<EnemyComponent>().sinceLastSeen += Clock :: delta;
                e->getComponent<EnemyComponent>().chase = false; // Something is blocking
            } else {
                e->getComponent<EnemyComponent>().sinceLastSeen = 0;
                e->getComponent<EnemyComponent>().lastSeenPosX = player.getComponent<TransformComponent>().position.x + player.getComponent<TransformComponent>().width / 2;
                e->getComponent<EnemyComponent>().lastSeenPosY = player.getComponent<TransformComponent>().position.y + player.getComponent<TransformComponent>().height / 2;
                e->getComponent<EnemyComponent>().chase = true; // Enemy sees player
            }
        }
    }
    

    for (auto& pet : pets) {
        Collision::CheckCollisions(*pet, pets);
        if (Collision::AABB(player.getComponent<ColiderComponent>(), pet->getComponent<ColiderComponent>()) && (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F)) {
            pet->getComponent<PetComponent>().follow = !pet->getComponent<PetComponent>().follow;
            player.getComponent<PlayerComponent>().animals_saved++;
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
            static float prevX;
            static float prevY;
            if (!inDungeon) {
                prevX = lab.getComponent<TransformComponent>().position.x;
                prevY = lab.getComponent<TransformComponent>().position.y;
            }
            inDungeon = !inDungeon;
            changeMap(prevX, prevY);
        }
    }

    for (auto& e : enemies) {
        if (Collision :: AABB(player.getComponent<ColiderComponent>(), e->getComponent<ColiderComponent>()) && player.getComponent<TransformComponent>().knockbackTime <= 0) {
            Collision :: applyKnockback(player, *e, 3);
            player.getComponent<HPComponent>().healthPoints--;
        }
    }
    player.getComponent<TransformComponent>().knockbackTime -= Clock :: delta;

    if (player.getComponent<TransformComponent>().isKnockbacked) {
        // Simple damping
        player.getComponent<TransformComponent>().velocity.x *= 0.9f;
        player.getComponent<TransformComponent>().velocity.y *= 0.9f;

        // Stop if very slow
        if (std::abs(player.getComponent<TransformComponent>().velocity.x) < 0.1f && std::abs(player.getComponent<TransformComponent>().velocity.y) < 0.1f) {
            player.getComponent<TransformComponent>().velocity.x = 0;
            player.getComponent<TransformComponent>().velocity.y = 0;
            player.getComponent<TransformComponent>().isKnockbacked = false;
        }
    }


    GameLogic::CameraSystem(player);

    Collision::CheckCollisions(player, tiles);
    for (auto& e : enemies) {
        Collision::CheckCollisions(*e, tiles);
    }
    for (auto& p : pets) {
        Collision::CheckCollisions(*p, tiles);
    }

}


void Game :: render() {
    SDL_RenderClear(renderer);

    for (auto& t : tiles) t->draw();
    for (auto& t : turrets) t->draw();
    for (auto& p : pets) p->draw();
    for (auto& p : players) p->draw();
    for (auto& e : enemies) e->draw();
    for (auto& p : projectiles) p->draw();

    SDL_Rect srcRect = { 0,0,14,14 };
    SDL_Rect destRect;
    for (int i = 1; i <= player.getComponent<PlayerComponent>().max_ammo; i++) {
        destRect = {SCREEN_WIDTH - i * 35,50,28,28 };
        if (i > player.getComponent<PlayerComponent>().ammo_count) {
            SDL_SetTextureAlphaMod(bulletTexture, 50);
        }
        Texture::Draw(bulletTexture, srcRect, destRect, SDL_FLIP_NONE);
        SDL_SetTextureAlphaMod(bulletTexture, 255);

    }

    srcRect = { 0,0,640,640 };
    for (int i = 1; i <= player.getComponent<HPComponent>().maxHealthPoints; i++) {
        destRect = {SCREEN_WIDTH - i * 55,10,35,35 };
        if (i > player.getComponent<HPComponent>().healthPoints) {
            SDL_SetTextureAlphaMod(hpTexture, 50);
        }
        Texture::Draw(hpTexture, srcRect, destRect, SDL_FLIP_NONE);
        SDL_SetTextureAlphaMod(hpTexture, 255);
    }
    srcRect = { 0,0,32,32 };
    for (int i = 1; i <= all_pets; i++) {
        destRect = {10 + (i-1) * 35,10,28,28 };
        if (i > player.getComponent<PlayerComponent>().animals_saved) {
            SDL_SetTextureAlphaMod(savedPet, 50);
        }
        Texture::Draw(savedPet, srcRect, destRect, SDL_FLIP_NONE);
        SDL_SetTextureAlphaMod(savedPet, 255);
    }

    if (!player.isActive()) {
        srcRect = { 0,0,1028,1028 };
        destRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        Texture::Draw(game_overTexture, srcRect, destRect, SDL_FLIP_NONE);
    }



    if (player.getComponent<PlayerComponent>().animals_saved == all_pets && !inDungeon) {
        srcRect = { 0,0,1028,1028 };
        destRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        Texture::Draw(winTexture, srcRect, destRect, SDL_FLIP_NONE);
    }


    SDL_RenderPresent(renderer);

}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

