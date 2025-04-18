#include"game.hpp"
#include "Map.hpp"
#include"ECS/ECS.hpp"
#include <iostream>
#include <ostream>
#include"GameLogic.hpp"
#include"Collision.hpp"
#include<ctime>
#include<fstream>
#include<filesystem>


#include "ECS/Components.hpp"
#include"Clock.hpp"
#include "ECS/buttonComponent.hpp"
#include "ECS/TurretComponent.hpp"


struct saveGame {
    float playerX, playerY;
    int playerHealth, playerAmmo, animalsSaved;
    bool inDungeon, level1cleared;
};

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    OPTIONS,
    GAME_OVER
};

GameState currentState = GameState::MAIN_MENU;

bool Game :: level1Cleared = false;
bool Game :: level2Cleared = false;

int Game :: SCREEN_WIDTH = 1080;
int Game :: SCREEN_HEIGHT = 720;

float Game :: scaleX = 1;
float Game :: scaleY = 1;

bool Game :: clicked = false;
bool Game :: inDungeon = false;
int Game :: all_pets = -1;

SDL_Renderer* Game :: renderer = nullptr;
SDL_Event Game :: event;

SDL_Rect Game :: camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager manager;

std::vector<ColiderComponent*> Game :: colliders;

std::vector<Entity*> bulletPool;
std::vector<Entity*> enemyPool;
std::vector<Entity*> petPool;

int Map::sizeX = 60;
int Map::sizeY = 60;

std::string  mapFile = "assets/map_tiles.png";

auto& load_button(manager.addEntity());
auto& save_button(manager.addEntity());
auto& resume_button(manager.addEntity());
auto& back_to_menu(manager.addEntity());
auto& resize_small_button(manager.addEntity());
auto& resize_medium_button(manager.addEntity());
auto& resize_large_button(manager.addEntity());
auto& play_button(manager.addEntity());
auto& quit_button(manager.addEntity());
auto& options_button(manager.addEntity());

auto& player(manager.addEntity());
auto& enemy(manager.addEntity());
auto& enemy2(manager.addEntity());
auto& enemy3(manager.addEntity());
auto& enemy4(manager.addEntity());
auto& enemy5(manager.addEntity());
auto& enemy6(manager.addEntity());
auto& enemy7(manager.addEntity());
auto& enemy8(manager.addEntity());
auto& enemy9(manager.addEntity());
auto& enemy10(manager.addEntity());

auto& pet1(manager.addEntity());
auto& pet2(manager.addEntity());
auto& pet3(manager.addEntity());
auto& pet4(manager.addEntity());
auto& pet5(manager.addEntity());
auto& turret(manager.addEntity());
auto& lab(manager.addEntity());



auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& pets(manager.getGroup(groupPet));
auto& projectiles(manager.getGroup(groupProjectile));
auto& turrets = manager.getGroup(groupTurrets);


void Game :: init(const char* title, int xpos, int ypos, bool fullscreen) {
    if (fullscreen) {
        SCREEN_WIDTH = 1920;
        SCREEN_HEIGHT = 1080;
    }
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
        button_play = Texture::LoadTexture("assets/play_button.png");
        mainmenuscreen = Texture::LoadTexture("assets/mainmenu.png");
        button_exit = Texture::LoadTexture("assets/exit_button.png");
        button_options = Texture::LoadTexture("assets/options_button.png");
        pauseMenu = Texture::LoadTexture("assets/pauseMenu.png");
        button_small = Texture::LoadTexture("assets/small_button.png");
        button_medium = Texture::LoadTexture("assets/medium_button.png");
        button_large = Texture::LoadTexture("assets/large_button.png");
        button_resume = Texture::LoadTexture("assets/resume_button.png");
        button_back_to_menu = Texture::LoadTexture("assets/back_to_menu_button.png");
        options = Texture::LoadTexture("assets/background_options.png");
        button_save_game = Texture::LoadTexture("assets/save_game_button.png");
        button_load_game = Texture::LoadTexture("assets/load_game_button.png");

        Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);

        lab.addComponent<TransformComponent>(rand()%500+1000, rand()%500+1000, 16, 16, 2);
        lab.addComponent<SpriteComponent>("assets/lojtra.png");
        lab.addComponent<ColiderComponent>("lab");
        lab.addGroup(groupMap);

        player.addComponent<TransformComponent>(256, 132, 28, 17, 2, 10);
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
void Game :: addBullet(Entity* object, float targetX, float targetY) {
    float x = object->getComponent<TransformComponent>().position.x + object->getComponent<TransformComponent>().width/2;
    float y = object->getComponent<TransformComponent>().position.y + object->getComponent<TransformComponent>().height/2;

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


void Game::loadDungeon() {
    tiles.clear();
    mapFile = "assets/lab_tiles.png";
    if (!level1Cleared) {
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
        enemy.addComponent<HPComponent>(3);
        enemy.addGroup(groupEnemies);

        enemy2.addComponent<TransformComponent>(1500, 300, 41, 62, 1, 1);
        enemy2.addComponent<SpriteComponent>("assets/wolf.png", true);
        enemy2.addComponent<ColiderComponent>("wolf");
        enemy2.addComponent<EnemyComponent>(&player, "wolf");
        enemy2.addComponent<HPComponent>(3);
        enemy2.addGroup(groupEnemies);

        enemy3.addComponent<TransformComponent>(220, 1400, 30, 22, 2, 1);
        enemy3.addComponent<SpriteComponent>("assets/enemy.png", true);
        enemy3.addComponent<ColiderComponent>("enemy");
        enemy3.addComponent<EnemyComponent>(&player, "enemy");
        enemy3.addComponent<HPComponent>(3);
        enemy3.addGroup(groupEnemies);

        enemy4.addComponent<TransformComponent>(1540, 860, 41, 62, 1, 1);
        enemy4.addComponent<SpriteComponent>("assets/wolf.png", true);
        enemy4.addComponent<ColiderComponent>("wolf");
        enemy4.addComponent<EnemyComponent>(&player, "wolf");
        enemy4.addComponent<HPComponent>(3);
        enemy4.addGroup(groupEnemies);

        enemy5.addComponent<TransformComponent>(840, 1500, 30, 22, 2, 1);
        enemy5.addComponent<SpriteComponent>("assets/enemy.png", true);
        enemy5.addComponent<ColiderComponent>("enemy");
        enemy5.addComponent<EnemyComponent>(&player, "enemy");
        enemy5.addComponent<HPComponent>(3);
        enemy5.addGroup(groupEnemies);

        pet1.addComponent<TransformComponent>(380, 600, 32, 32, 1, 2);
        pet1.addComponent<SpriteComponent>("assets/pet.png", true);
        pet1.addComponent<ColiderComponent>("pet");
        pet1.addComponent<PetComponent>(&player);
        pet1.addGroup(groupPet);

        pet2.addComponent<TransformComponent>(1500, 260, 32, 32, 1, 2);
        pet2.addComponent<SpriteComponent>("assets/pet2.png", true);
        pet2.addComponent<ColiderComponent>("pet");
        pet2.addComponent<PetComponent>(&player);
        pet2.addGroup(groupPet);

        pet3.addComponent<TransformComponent>(950, 1510, 32, 32, 1, 2);
        pet3.addComponent<SpriteComponent>("assets/pet2.png", true);
        pet3.addComponent<ColiderComponent>("pet");
        pet3.addComponent<PetComponent>(&player);
        pet3.addGroup(groupPet);

        pet4.addComponent<TransformComponent>(1550, 820, 32, 32, 1, 2);
        pet4.addComponent<SpriteComponent>("assets/pet.png", true);
        pet4.addComponent<ColiderComponent>("pet");
        pet4.addComponent<PetComponent>(&player);
        pet4.addGroup(groupPet);

        pet5.addComponent<TransformComponent>(270, 1420, 32, 32, 1, 2);
        pet5.addComponent<SpriteComponent>("assets/pet.png", true);
        pet5.addComponent<ColiderComponent>("pet");
        pet5.addComponent<PetComponent>(&player);
        pet5.addGroup(groupPet);

        turret.addComponent<TransformComponent>(450, 650, 32, 32, 1);
        turret.addComponent<SpriteComponent>("assets/dirt.png");
        turret.addComponent<ColiderComponent>("turret");
        turret.addComponent<TurretComponent>(&player);
        turret.addGroup(groupTurrets);
    }
    else {
        Map::LoadMap("assets/level2.txt", Map::sizeX, Map::sizeY);

        player.getComponent<TransformComponent>().position.x = 940;
        player.getComponent<TransformComponent>().position.y = 1130;

        lab.getComponent<TransformComponent>().position.x = 80;
        lab.getComponent<TransformComponent>().position.y = 930;
        lab.addGroup(groupMap);


        enemy6.addComponent<TransformComponent>(1100, 140, 30, 22, 2, 1);
        enemy6.addComponent<SpriteComponent>("assets/enemy.png", true);
        enemy6.addComponent<ColiderComponent>("enemy");
        enemy6.addComponent<EnemyComponent>(&player, "enemy");
        enemy6.addComponent<HPComponent>(3);
        enemy6.addGroup(groupEnemies);


        enemy7.addComponent<TransformComponent>(220, 470, 41, 62, 1, 1);
        enemy7.addComponent<SpriteComponent>("assets/wolf.png", true);
        enemy7.addComponent<ColiderComponent>("wolf");
        enemy7.addComponent<EnemyComponent>(&player, "wolf");
        enemy7.addComponent<HPComponent>(3);
        enemy7.addGroup(groupEnemies);


        enemy8.addComponent<TransformComponent>(1750, 370, 30, 22, 2, 1);
        enemy8.addComponent<SpriteComponent>("assets/enemy.png", true);
        enemy8.addComponent<ColiderComponent>("enemy");
        enemy8.addComponent<EnemyComponent>(&player, "enemy");
        enemy8.addComponent<HPComponent>(3);
        enemy8.addGroup(groupEnemies);


        enemy9.addComponent<TransformComponent>(320, 1650, 41, 62, 1, 1);
        enemy9.addComponent<SpriteComponent>("assets/wolf.png", true);
        enemy9.addComponent<ColiderComponent>("wolf");
        enemy9.addComponent<EnemyComponent>(&player, "wolf");
        enemy9.addComponent<HPComponent>(3);
        enemy9.addGroup(groupEnemies);


        enemy10.addComponent<TransformComponent>(1580, 1700, 30, 22, 2, 1);
        enemy10.addComponent<SpriteComponent>("assets/enemy.png", true);
        enemy10.addComponent<ColiderComponent>("enemy");
        enemy10.addComponent<EnemyComponent>(&player, "enemy");
        enemy10.addComponent<HPComponent>(3);
        enemy10.addGroup(groupEnemies);

        pet1.getComponent<TransformComponent>().position.x = 250;
        pet1.getComponent<TransformComponent>().position.y = 550;
        pet1.getComponent<PetComponent>().saved = false;
        pet1.addGroup(groupPet);


        pet2.getComponent<TransformComponent>().position.x = 1060;
        pet2.getComponent<TransformComponent>().position.y = 175;
        pet2.getComponent<PetComponent>().saved = false;
        pet2.addGroup(groupPet);


        pet3.getComponent<TransformComponent>().position.x = 165;
        pet3.getComponent<TransformComponent>().position.y = 1670;
        pet3.getComponent<PetComponent>().saved = false;
        pet3.addGroup(groupPet);


        pet4.getComponent<TransformComponent>().position.x = 1600;
        pet4.getComponent<TransformComponent>().position.y = 1700;
        pet4.getComponent<PetComponent>().saved = false;
        pet4.addGroup(groupPet);


        pet5.getComponent<TransformComponent>().position.x = 1800;
        pet5.getComponent<TransformComponent>().position.y = 350;
        pet5.getComponent<PetComponent>().saved = false;
        pet5.addGroup(groupPet);


        /*enemy.getComponent<TransformComponent>().position.x = 1100;
        enemy.getComponent<TransformComponent>().position.y = 140;
        enemy.getComponent<HPComponent>().healthPoints = 3;
        enemy.reactivate();
        enemy.addGroup(groupEnemies);


        enemy2.getComponent<TransformComponent>().position.x = 220;
        enemy2.getComponent<TransformComponent>().position.y = 470;
        enemy2.getComponent<HPComponent>().healthPoints = 3;
        enemy2.reactivate();
        enemy2.addGroup(groupEnemies);


        enemy3.getComponent<TransformComponent>().position.x = 1750;
        enemy3.getComponent<TransformComponent>().position.y = 370;
        enemy3.getComponent<HPComponent>().healthPoints = 3;
        enemy3.reactivate();
        enemy3.addGroup(groupEnemies);


        enemy4.getComponent<TransformComponent>().position.x = 320;
        enemy4.getComponent<TransformComponent>().position.y = 1650;
        enemy4.getComponent<HPComponent>().healthPoints = 3;
        enemy4.reactivate();
        enemy4.addGroup(groupEnemies);


        enemy5.getComponent<TransformComponent>().position.x = 1580;
        enemy5.getComponent<TransformComponent>().position.y = 1700;
        enemy5.getComponent<HPComponent>().healthPoints = 3;
        enemy5.reactivate();
        enemy5.addGroup(groupEnemies);*/
    }
}
void Game::backToIsland(float prevX, float prevY) {
    mapFile = "assets/map_tiles.png";
    tiles.clear();
    enemies.clear();
    pets.clear();
    turrets.clear();
    Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);

    player.getComponent<TransformComponent>().position.x = prevX;
    player.getComponent<TransformComponent>().position.y = prevY;

    lab.getComponent<TransformComponent>().position.x = rand()%100+500;
    lab.getComponent<TransformComponent>().position.y = rand()%100+500;
    lab.addGroup(groupMap);
}


void Game::handleEvents() {
    static int max_ammo = 5;
    static int ammo_reload = 0;
    bool mainMenuinitialized = false;
    bool optionsInitialized = false;
    bool pauseInitialized = false;

    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
             isRunning = false;
        break;
        default:
            break;
    }
    switch (currentState) {
        case GameState::MAIN_MENU:
            if (!mainMenuinitialized) {
                initMainMenu();
                mainMenuinitialized = true;
            }
            updateMainMenu();
            renderMainMenu();
        break;

        case GameState::PLAYING:
            update();
            render();
        break;

        case GameState::OPTIONS:
           if (!optionsInitialized) {
               initOptions();
               optionsInitialized = true;
           }
           updateOptions();
           renderOptions();
        break;

        case GameState::PAUSED:
            if (!pauseInitialized) {
                initPause();
                pauseInitialized = true;
            }
            updatePause();
            renderPause();
        break;
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && currentState == GameState::PLAYING) {
        currentState = GameState::PAUSED;
    }
    else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && currentState == GameState::OPTIONS) {
        currentState = GameState::MAIN_MENU;
    }

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

    if (player.getComponent<PlayerComponent>().ammo_count > 0 && clicked && inDungeon && currentState == GameState::PLAYING) {
        Game::addBullet(&player, (float)event.button.x, (float)event.button.y);
        player.getComponent<PlayerComponent>().ammo_count--;
        clicked = false;
    }
    if (player.getComponent<PlayerComponent>().ammo_count == max_ammo) {
        ammo_reload = 0;
    }
    else if (ammo_reload > 2000){
        player.getComponent<PlayerComponent>().ammo_count++;
        ammo_reload = 0;
    }
    ammo_reload += Clock :: delta;
}

void Game :: update() {
    manager.refresh();
    manager.update();

    all_pets = pets.size();
    //all_pets = 5;

    scaleX = (float)SCREEN_WIDTH / 1080;
    scaleY = (float)SCREEN_HEIGHT / 720;

    /*for (auto& t : tiles) {
        if (t->hasComponent<ColiderComponent>()) {
            if (Collision::AABB(lab.getComponent<ColiderComponent>().collider, t->getComponent<ColiderComponent>().collider)) {
                lab.getComponent<TransformComponent>().position.x = rand()%500+1000;
                lab.getComponent<TransformComponent>().position.y = rand()%500+1000;
            }
        }
    }*/





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
        if (pet->isActive()) {
            Collision::CheckCollisions(*pet, pets);
            if (Collision::AABB(player.getComponent<ColiderComponent>(), pet->getComponent<ColiderComponent>()) && (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F)) {
                pet->getComponent<PetComponent>().follow = true;
                //pet->destroy();
                if (!pet->getComponent<PetComponent>().saved) {
                    pet->getComponent<PetComponent>().saved = true;
                    player.getComponent<PlayerComponent>().animals_saved++;
                    //petPool.push_back(pet);
                }
            }
            if(Collision::AABB(player.getComponent<ColiderComponent>(), pet->getComponent<ColiderComponent>())) {
                pet->getComponent<TransformComponent>().velocity.x = 0;
                pet->getComponent<TransformComponent>().velocity.y = 0;
                pet->getComponent<SpriteComponent>().play("pet_idle");
            }
        }
    }

    for (auto& p : projectiles) {
        for (auto& e : enemies) {
            if (e->isActive() && p->isActive()) {
                if (Collision::AABB(p->getComponent<ColiderComponent>(), e->getComponent<ColiderComponent>())) {
                    p->deactivate();
                    bulletPool.push_back(p);
                    //e->deactivate();
                    e->getComponent<HPComponent>().healthPoints--;
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

    static float prevX;
    static float prevY;

    if (Collision::AABB(player.getComponent<ColiderComponent>(), lab.getComponent<ColiderComponent>())) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F) {

            if (!inDungeon) {
                prevX = lab.getComponent<TransformComponent>().position.x;
                prevY = lab.getComponent<TransformComponent>().position.y;
                loadDungeon();
            }
            else{
                if (level1Cleared) {
                    level2Cleared = true;
                }
                level1Cleared = true;
                backToIsland(prevX, prevY);
            }
            inDungeon = !inDungeon;
        }
    }

    for (auto& e : enemies) {
        if (e->isActive()) {
            if (Collision :: AABB(player.getComponent<ColiderComponent>(), e->getComponent<ColiderComponent>()) && player.getComponent<TransformComponent>().knockbackTime <= 0) {
                Collision :: applyKnockback(player, *e, 3);
                player.getComponent<HPComponent>().healthPoints--;
            }
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
void Game::initMainMenu() {
    play_button.addComponent<buttonComponent>(100 + scaleX * 30, 50 * scaleY + scaleY * 200, 1024, 1024, "assets/play_button");
    quit_button.addComponent<buttonComponent>(100 + scaleX * 30, 100 * scaleY + scaleY * 275, 1024, 1024, "assets/exit_button");
    options_button.addComponent<buttonComponent>(100 + scaleX * 30, 150 * scaleY + scaleY * 350, 1024, 1024, "assets/options_button");
    load_button.addComponent<buttonComponent>(100 + 30 * scaleX, 200 * scaleY + scaleY * 425, 1024, 1024, "assets/load_game_button");

}

void Game::updateMainMenu() {
    SDL_SetTextureAlphaMod(button_play, 255);
    SDL_SetTextureAlphaMod(button_exit, 255);
    SDL_SetTextureAlphaMod(button_options, 255);
    SDL_SetTextureAlphaMod(button_load_game, 255);

    SDL_Rect mouse {0, 0, 1, 1};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    if (Collision::AABB(play_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_play, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_play, 100);
        if (clicked)
        currentState = GameState::PLAYING;
    }
    else if (Collision::AABB(quit_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_exit, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_exit, 50);
        if (clicked)
            isRunning = false;

    }
    else if (Collision::AABB(options_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_options, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_options, 100);
        if (clicked)
            currentState = GameState::OPTIONS;
    }
    else if (Collision::AABB(load_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_load_game, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_load_game, 100);
        if (clicked){
            loadGame();
            currentState = GameState::PLAYING;
        }
    }
}
void Game::renderMainMenu() {
    SDL_RenderClear(renderer);
    static int x = 30;

    SDL_Rect srcRect{0,0,1920,1280}, destRect{0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_SetTextureAlphaMod(mainmenuscreen, 100);

    Texture::Draw(mainmenuscreen, srcRect, destRect, SDL_FLIP_NONE);
    Texture::Draw(button_play, play_button.getComponent<buttonComponent>().srcRect, play_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);
    Texture::Draw(button_exit, quit_button.getComponent<buttonComponent>().srcRect, quit_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);
    Texture::Draw(button_options, options_button.getComponent<buttonComponent>().srcRect, options_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);
    Texture::Draw(button_load_game, load_button.getComponent<buttonComponent>().srcRect, load_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
}

void Game::initOptions() {
    int xpos = SCREEN_WIDTH / 2 ;
    int ypos = SCREEN_HEIGHT / 3;

    resize_small_button.addComponent<buttonComponent>(xpos - 150 * scaleX / 2, ypos, 1024, 1024, "assets/play_button");
    resize_medium_button.addComponent<buttonComponent>(xpos - 150 * scaleX / 2, ypos + 120 * scaleY, 1024, 1024, "assets/play_button");
    resize_large_button.addComponent<buttonComponent>(xpos - 150 * scaleX / 2, ypos + 120 * 2 * scaleY, 1024, 1024, "assets/play_button");
}
void Game::updateOptions() {
    SDL_Rect mouse {0, 0, 1, 1};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    SDL_SetTextureAlphaMod(button_play, 255);

    if (Collision::AABB(resize_small_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_play, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_play, 100);
        if (clicked) {
            SCREEN_WIDTH = 1080;
            SCREEN_HEIGHT = 720;
            SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
            centerWindow(window);
        }
    }
    else if (Collision::AABB(resize_medium_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_play, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_play, 100);
        if (clicked) {
            SCREEN_WIDTH = 1440;
            SCREEN_HEIGHT = 960;
            SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
            centerWindow(window);
        }
    }
    else if (Collision::AABB(resize_large_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_play, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_play, 100);
        if (clicked) {
            SCREEN_WIDTH = 1800;
            SCREEN_HEIGHT = 1024;
            SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
            centerWindow(window);
        }
    }
}
void Game::renderOptions() {
    SDL_RenderClear(renderer);
    SDL_Rect srcRect = { 0, 0, 1024, 1024 };
    SDL_Rect destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    Texture::Draw(options, srcRect, destRect, SDL_FLIP_NONE);
    Texture::Draw(button_small, resize_small_button.getComponent<buttonComponent>().srcRect, resize_small_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);
    Texture::Draw(button_medium, resize_medium_button.getComponent<buttonComponent>().srcRect, resize_medium_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);
    Texture::Draw(button_large, resize_large_button.getComponent<buttonComponent>().srcRect, resize_large_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
}

void Game::initPause() {
    int posX = SCREEN_WIDTH / 2;
    int posY = SCREEN_HEIGHT / 3;

    resume_button.addComponent<buttonComponent>(posX - 150 * scaleX / 2, posY, 1024, 1024, "assets/play_button");
    back_to_menu.addComponent<buttonComponent>(posX - 150 * scaleX / 2,posY + 120 * scaleY, 1024, 1024, "assets/play_button");
    save_button.addComponent<buttonComponent>(posX - 150 * scaleX / 2, posY + 2* 120 * scaleY, 1024, 1024, "assets/play_button");
}

void Game::updatePause() {
    SDL_Rect mouse {0, 0, 1, 1};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    SDL_SetTextureAlphaMod(button_resume, 255);
    SDL_SetTextureAlphaMod(button_back_to_menu, 255);
    SDL_SetTextureAlphaMod(button_save_game, 255);

    if (Collision::AABB(resume_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_resume, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_resume, 100);
        if (clicked)
            currentState = GameState::PLAYING;
    }
    if (Collision::AABB(back_to_menu.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_back_to_menu, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_back_to_menu, 100);
        if (clicked)
            currentState = GameState::MAIN_MENU;
    }
    if (Collision::AABB(save_button.getComponent<buttonComponent>().destRect, mouse)) {
        SDL_SetTextureBlendMode(button_save_game, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(button_save_game, 100);
        if (clicked)
            saveGame();
    }
}

void Game::renderPause() {
    SDL_RenderClear(renderer);
    SDL_SetTextureAlphaMod(pauseMenu, 40);
    SDL_Rect srcRect{0,0,200,200}, destRect{0,0,SCREEN_WIDTH, SCREEN_HEIGHT};

    Texture::Draw(pauseMenu, srcRect, destRect, SDL_FLIP_NONE );
    Texture::Draw(button_resume, resume_button.getComponent<buttonComponent>().srcRect, resume_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);
    Texture::Draw(button_back_to_menu, back_to_menu.getComponent<buttonComponent>().srcRect, back_to_menu.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);
    Texture::Draw(button_save_game, save_button.getComponent<buttonComponent>().srcRect, save_button.getComponent<buttonComponent>().destRect, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
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



    if (level2Cleared) {
        srcRect = { 0,0,1028,1028 };
        destRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        Texture::Draw(winTexture, srcRect, destRect, SDL_FLIP_NONE);
    }


    SDL_RenderPresent(renderer);

}
void Game::centerWindow(SDL_Window* window) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(0, &displayBounds);

    int x = (displayBounds.w - windowWidth) / 2;
    int y = (displayBounds.h - windowHeight) / 2;

    SDL_SetWindowPosition(window, x, y);
}

void Game::saveGame() {
    std::ofstream saveFile("saved_game.bin", std::ios::binary);
    std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
    struct saveGame data;
    data.playerX = player.getComponent<TransformComponent>().position.x;
    data.playerY = player.getComponent<TransformComponent>().position.y;
    data.playerAmmo = player.getComponent<PlayerComponent>().ammo_count;
    data.playerHealth = player.getComponent<HPComponent>().healthPoints;
    data.animalsSaved = player.getComponent<PlayerComponent>().animals_saved;
    data.level1cleared = level1Cleared;
    data.inDungeon = inDungeon;
    saveFile.write((char*)&data, sizeof(data));
    saveFile.close();
}
void Game::loadGame() {
    std::ifstream saveFile("saved_game.bin", std::ios::binary);
    if (saveFile.is_open()) {
        std::cout<<"save file opened"<<std::endl;
        struct saveGame data;
        saveFile.read((char*)&data, sizeof(data));
        player.getComponent<TransformComponent>().position.x = data.playerX;
        player.getComponent<TransformComponent>().position.y = data.playerY;
        player.getComponent<PlayerComponent>().ammo_count = data.playerAmmo;
        player.getComponent<HPComponent>().healthPoints = data.playerHealth;
        player.getComponent<PlayerComponent>().animals_saved = data.playerAmmo;
        level1Cleared = data.level1cleared;
        inDungeon = data.inDungeon;
        if (!inDungeon) {
            tiles.clear();
            mapFile = "assets/map_tiles.png";
            Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);
        }
        else if (inDungeon && level1Cleared) {
            loadDungeon();
            //tiles.clear();
            //mapFile = "assets/lab_tiles.png";
            //Map::LoadMap("assets/level2.txt", Map::sizeX, Map::sizeY);
        }//
        else if (inDungeon) {
            loadDungeon();
            //tiles.clear();
            //mapFile = "assets/lab_tiles.png";
            //Map::LoadMap("assets/lab.txt", Map::sizeX, Map::sizeY);
        }
    }
    std::cout<<"save file closed"<<std::endl;
    saveFile.close();
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

