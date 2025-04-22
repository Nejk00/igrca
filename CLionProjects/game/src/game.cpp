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
#include"ECS/TurretComponent.hpp"

struct enemy {
    float posX, posY;
    int healthpoints;
    bool alive;
};
struct pet {
    float posX, posY;
    bool saved;
};
struct users {
    char name[12];
    int score;
};
struct replayGame {
    float velocityX, velocityY, speed;
    int changeMap;
};

struct saveGame {
    float playerX, playerY, prevX, prevY, labX, labY;
    int playerHealth, playerAmmo, animalsSaved, score;
    bool inDungeon, level1cleared;
    char username[12];

};
struct saveGame2 {
    float playerX, playerY, prevX, prevY, labX, labY;
    int playerHealth, playerAmmo, animalsSaved, score, petCount, enemyCount;
    bool inDungeon, level1cleared;
    char username[12];

};

enum class GameState {
    USER_INPUT,
    REPLAY,
    MAIN_MENU,
    PLAYING,
    PAUSED,
    OPTIONS,
    GAME_OVER
};

GameState currentState = GameState::MAIN_MENU;

bool Game::game_over = false;
float Game:: prevX = 0, Game:: prevY = 0;
bool Game:: replayFinished = false;
bool Game::resetReplay = false;
bool Game :: level1Cleared = false;
bool Game :: level2Cleared = false;
bool Game::highscoreUpdated = false;
int Game :: score = 0;
float Game :: scaleX = 1, Game::scaleY = 1;

int Game :: SCREEN_WIDTH = 1080;
int Game :: SCREEN_HEIGHT = 720;
int Game :: LETTER_WIDTH = 16;
int Game :: LETTER_HEIGHT = 16;
int Game:: flags = 0;

bool Game :: clicked = false;
bool Game :: inDungeon = false;
int Game :: all_pets = -1;

SDL_Texture* Game::numbersTexture = Texture::LoadTexture("assets/numbers.png");
SDL_Texture* Game::lettersTexture = Texture::LoadTexture("assets/letters.png");

SDL_Renderer* Game :: renderer = nullptr;
SDL_Event Game :: event;

SDL_Rect Game :: camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager manager;

std::vector<ColiderComponent*> Game :: colliders;

std::vector<replayGame > replayVector;
std::vector<Entity*> bulletPool;
std::vector<Entity*> enemyPool;
std::vector<Entity*> petPool;
static std::vector<replayGame> replayData;

int Map::sizeX = 60;
int Map::sizeY = 60;

std::string  mapFile = "assets/map_tiles.png";

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
auto& pet6(manager.addEntity());
auto& pet7(manager.addEntity());
auto& pet8(manager.addEntity());
auto& pet9(manager.addEntity());
auto& pet10(manager.addEntity());

auto& lab(manager.addEntity());



auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& pets(manager.getGroup(groupPet));
auto& projectiles(manager.getGroup(groupProjectile));



void Game :: init(const char* title, int xpos, int ypos, bool fullscreen) {
    if (fullscreen) {
        SCREEN_WIDTH = 1920;
        SCREEN_HEIGHT = 1080;
    }
    srand(time(NULL));
    //int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL_Init OK" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, SCREEN_WIDTH, SCREEN_HEIGHT, flags | SDL_WINDOW_RESIZABLE);
        if (window) {
            std::cout << "SDL_CreateWindow OK" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderSetLogicalSize(renderer, 1080, 720);
            std::cout << "SDL_CreateRenderer OK" << std::endl;
        }
        isRunning = true;
    }
    else {
        isRunning = false;
    }
        std::ofstream replay("replay.bin", std::ios::binary);
        replay.close();

        bulletTexture = Texture::LoadTexture("assets/player_bullet.png");
        hpTexture = Texture::LoadTexture("assets/heart.png");
        savedPet = Texture::LoadTexture("assets/pet.png");
        mainmenuscreen = Texture::LoadTexture("assets/mainmenu.png");
        numbersTexture = Texture::LoadTexture("assets/numbers.png");
        lettersTexture = Texture::LoadTexture("assets/letters.png");
        background = Texture::LoadTexture("assets/background.png");


        Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);

        lab.addComponent<TransformComponent>(rand()%300+800, rand()%500+800, 16, 16, 2);
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
    pets.clear();
    enemies.clear();
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

            pet6.addComponent<TransformComponent>(250, 550, 32, 32, 1, 2);
            pet6.addComponent<SpriteComponent>("assets/pet.png", true);
            pet6.addComponent<ColiderComponent>("pet");
            pet6.addComponent<PetComponent>(&player);
            pet6.addGroup(groupPet);

            pet7.addComponent<TransformComponent>(1060, 175, 32, 32, 1, 2);
            pet7.addComponent<SpriteComponent>("assets/pet2.png", true);
            pet7.addComponent<ColiderComponent>("pet");
            pet7.addComponent<PetComponent>(&player);
            pet7.addGroup(groupPet);

            pet8.addComponent<TransformComponent>(165, 1670, 32, 32, 1, 2);
            pet8.addComponent<SpriteComponent>("assets/pet2.png", true);
            pet8.addComponent<ColiderComponent>("pet");
            pet8.addComponent<PetComponent>(&player);
            pet8.addGroup(groupPet);

            pet9.addComponent<TransformComponent>(1600, 1700, 32, 32, 1, 2);
            pet9.addComponent<SpriteComponent>("assets/pet.png", true);
            pet9.addComponent<ColiderComponent>("pet");
            pet9.addComponent<PetComponent>(&player);
            pet9.addGroup(groupPet);

            pet10.addComponent<TransformComponent>(1800, 350, 32, 32, 1, 2);
            pet10.addComponent<SpriteComponent>("assets/pet.png", true);
            pet10.addComponent<ColiderComponent>("pet");
            pet10.addComponent<PetComponent>(&player);
            pet10.addGroup(groupPet);
        }
}
void Game::backToIsland(float prevX, float prevY) {
    mapFile = "assets/map_tiles.png";
    tiles.clear();
    enemies.clear();
    pets.clear();
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
    static bool ReplaySaved = false;
    static bool ReplayInitialized = false;

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
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                currentState = GameState::USER_INPUT;
            }
            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_L) {
                loadGame();
                currentState = GameState::PLAYING;
            }
            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_R) {
                currentState = GameState::REPLAY;
            }

            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_E) {
                isRunning = false;
            }
            renderMainMenu();
        break;

        case GameState::PLAYING:
            update();
            render();
        break;

        case GameState::PAUSED:
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_R) {
                currentState = GameState::PLAYING;
            }
            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                currentState = GameState::MAIN_MENU;
            }
            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_S) {
                saveGame();
            }
            renderPause();
        break;

        case GameState::REPLAY:
            if (!ReplayInitialized) {
                initReplay();
                ReplayInitialized = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_R) {
                resetReplay = true;
            }
            replay();
            renderReplay();
        break;
        case GameState::GAME_OVER:
            if (!highscoreUpdated) {
                calculateScore();
                highscore(username, score);
                highscoreUpdated = true;
            }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            resetGame();
            currentState = GameState::PLAYING;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            resetGame();
            currentState = GameState::MAIN_MENU;
        }
        renderGameOver();
        break;

        case GameState::USER_INPUT:
            SDL_StartTextInput();

        if (event.type == SDL_TEXTINPUT) {
            if (username.length() < 12) {
                SDL_StartTextInput();
                username += event.text.text;
            }
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_BACKSPACE && !username.empty()) {
                username.pop_back();
            }
            if (event.key.keysym.sym == SDLK_RETURN && !username.empty()) {
                SDL_StopTextInput();
                currentState = GameState::PLAYING;
            }
        }
            renderUserInput();
        break;
    }

    if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && currentState == GameState::PLAYING) {
        currentState = GameState::PAUSED;
    }
    else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && (currentState == GameState::OPTIONS || currentState == GameState::REPLAY)) {
        currentState = GameState::MAIN_MENU;
    }
    if (game_over) {
        currentState = GameState::GAME_OVER;
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

    scaleX = (float)SCREEN_WIDTH / 1080;
    scaleY = (float)SCREEN_HEIGHT / 720;

    replayGame data;

    std::ofstream replayFile("replay.bin", std::ios::app | std::ios::binary);
    data.velocityX = player.getComponent<TransformComponent>().velocity.x;
    data.velocityY = player.getComponent<TransformComponent>().velocity.y;
    data.speed = player.getComponent<TransformComponent>().speed;
    data.changeMap = 0;

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
            //Collision::CheckCollisions(*pet, pets);
            if (Collision::AABB(player.getComponent<ColiderComponent>(), pet->getComponent<ColiderComponent>()) && (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F)) {
                //pet->getComponent<PetComponent>().follow = true;
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
                    //bulletPool.push_back(p);
                    //e->deactivate();
                    e->getComponent<HPComponent>().healthPoints--;
                    //enemyPool.push_back(e);
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

            if (!inDungeon) {
                if (!level1Cleared)
                    data.changeMap = 2;
                else
                    data.changeMap = 3;
                prevX = lab.getComponent<TransformComponent>().position.x;
                prevY = lab.getComponent<TransformComponent>().position.y;
                loadDungeon();
            }
            else{
                if (level1Cleared) {
                    level2Cleared = true;
                }
                level1Cleared = true;
                data.changeMap = 1;
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
    for (auto& e : enemies) {
        if (!e->isActive()) {
            player.getComponent<PlayerComponent>().enemiesKilled++;
        }
    }

    replayFile.write(reinterpret_cast<char*>(&data), sizeof(data));
    replayFile.close();

    if (level2Cleared)
        game_over = true;

    GameLogic::CameraSystem(player);

    Collision::CheckCollisions(player, tiles);
    for (auto& e : enemies) {
        Collision::CheckCollisions(*e, tiles);
    }
    for (auto& p : pets) {
        Collision::CheckCollisions(*p, tiles);
    }

}

void Game::renderMainMenu() {
    SDL_RenderClear(renderer);

    SDL_Rect srcRect{0,0,1920,1280}, destRect{0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_SetTextureAlphaMod(mainmenuscreen, 100);

    Texture::Draw(mainmenuscreen, srcRect, destRect, SDL_FLIP_NONE);

    Texture::RenderText("PRESS ENTER TO PLAY", 50, 250, 1);
    Texture::RenderText("PRESS L TO LOAD", 50, 350, 1);
    Texture::RenderText("PRESS R TO REPLAY", 50, 450, 1);
    Texture::RenderText("PRESS E TO EXIT", 50, 550, 1);

    SDL_RenderPresent(renderer);
}


void Game::renderPause() {
    SDL_RenderClear(renderer);
    //SDL_SetTextureAlphaMod(pauseMenu, 5);

    for (auto& t : tiles) t->draw();
    for (auto& p : pets) p->draw();
    for (auto& p : players) p->draw();
    for (auto& e : enemies) e->draw();
    for (auto& p : projectiles) p->draw();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // 128 = 50% opacity

    SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    Texture::RenderText("PRESS R TO RESUME", 350, 250, 1);
    Texture::RenderText("PRESS S TO SAVE", 350, 350, 1);
    Texture::RenderText("PRESS ESC FOR MENU", 350, 450, 1);

    //SDL_Rect srcRect{0, 0, 200, 200}, destRect{0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
    //Texture::Draw(pauseMenu, srcRect, destRect, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
}

void Game::renderGameOver() {
    SDL_RenderClear(renderer);

    SDL_Rect srcRect{0,0,1024,1024}, destRect{0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_SetTextureAlphaMod(background, 100);

    Texture::Draw(background, srcRect, destRect, SDL_FLIP_NONE);

    std::ifstream highscores("highscores.bin", std::ios::binary);
    users user;
    int counter = 0;
    while (highscores.read((char*)&user, sizeof(user)) && counter < 5) {
        Texture::RenderText(user.name,2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT / 4 + counter * 50 - 40, 1);
        Texture::RenderNumber(user.score, 2 * SCREEN_WIDTH / 3 + 200, SCREEN_HEIGHT / 4 + counter * 50 - 40, 1);
        counter++;
    }
    highscores.close();

    Texture::RenderText("USERNAME", 2 * SCREEN_WIDTH / 3 - 20, SCREEN_HEIGHT / 3 - 150, 1);
    Texture::RenderText("SCORE", 2 * SCREEN_WIDTH / 3 + 180, SCREEN_HEIGHT / 3 - 150, 1);
    Texture::RenderText("PRESS ENTER TO PLAY AGAIN", 50, 320, 1.2);
    Texture::RenderText("PRESS ESC FOR MENU", 50, 420, 1.2);

    SDL_RenderPresent(renderer);
}

void Game::renderUserInput() {
    SDL_RenderClear(renderer);

    SDL_Rect srcRect{0,0,1024,1024}, destRect{0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_SetTextureAlphaMod(background, 100);

    Texture::Draw(background, srcRect, destRect, SDL_FLIP_NONE);

    Texture::RenderText("ENTER USERNAME: ", 100, 200, 3);
    Texture::RenderText(username, SCREEN_WIDTH / 2 - username.size() * 20, SCREEN_HEIGHT / 2, 2);

    SDL_RenderPresent(renderer);
}

void Game :: render() {
    SDL_RenderClear(renderer);

    for (auto& t : tiles) t->draw();
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
    SDL_RenderPresent(renderer);

}

void Game::calculateScore() {
    score = player.getComponent<HPComponent>().healthPoints * 50 + player.getComponent<PlayerComponent>().animals_saved * 100 + player.getComponent<PlayerComponent>().enemiesKilled * 75;
    if (level1Cleared)
        score += 300;
    if (level2Cleared)
        score += 300;
}


void Game::saveGame() {
    std::ofstream saveFile("saved_game.bin", std::ios::binary);
    struct saveGame data;
    data.playerX = player.getComponent<TransformComponent>().position.x;
    data.playerY = player.getComponent<TransformComponent>().position.y;
    data.labX = lab.getComponent<TransformComponent>().position.x;
    data.labY = lab.getComponent<TransformComponent>().position.y;
    data.prevX = prevX;
    data.prevY = prevY;
    data.playerAmmo = player.getComponent<PlayerComponent>().ammo_count;
    data.playerHealth = player.getComponent<HPComponent>().healthPoints;
    data.animalsSaved = player.getComponent<PlayerComponent>().animals_saved;
    data.level1cleared = level1Cleared;
    data.inDungeon = inDungeon;
    strcpy(data.username, username.c_str());
    data.score = score;
    saveFile.write((char*)&data, sizeof(data));
    saveFile.close();
}
void Game::loadGame() {
    std::ifstream saveFile("saved_game.bin", std::ios::binary);
    if (saveFile.is_open()) {
        struct saveGame data;
        saveFile.read((char*)&data, sizeof(data));
        player.getComponent<TransformComponent>().position.x = data.playerX;
        player.getComponent<TransformComponent>().position.y = data.playerY;
        lab.getComponent<TransformComponent>().position.x = data.labX;
        lab.getComponent<TransformComponent>().position.y = data.labY;
        prevX = data.prevX;
        prevY = data.prevY;
        player.getComponent<PlayerComponent>().ammo_count = data.playerAmmo;
        player.getComponent<HPComponent>().healthPoints = data.playerHealth;
        player.getComponent<PlayerComponent>().animals_saved = data.playerAmmo;
        level1Cleared = data.level1cleared;
        inDungeon = data.inDungeon;
        username = data.username;
        score = data.score;

        /*if (!inDungeon) {
            tiles.clear();
            mapFile = "assets/map_tiles.png";
            Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);
        }*/
        if (inDungeon)
            loadDungeon();
    }

    saveFile.close();
}

void Game::initReplay() {
    enemies.clear();
    pets.clear();

    replayFile.open("replay.bin", std::ios::binary);
    if (!replayFile.is_open()) {
        std::cout << "Failed to open replay file!" << std::endl;
        return;
    }

    // Load all replay data at once
    replayGame data;
    while (replayFile.read((char*)&data, sizeof(data))) {
        replayData.push_back(data);
    }
    replayFile.close();

    player.getComponent<TransformComponent>().position.x = 256;
    player.getComponent<TransformComponent>().position.y = 132;
}
void Game :: saveReplay() {
    std::ofstream replay_file("replay.bin", std::ios::binary);
    replayGame data;
    for (int i=0; i<replayData.size(); ++i) {
        replay_file.write(reinterpret_cast<char*>(&data), sizeof(data));
    }
    replay_file.close();
}
void Game::replay() {
    manager.refresh();
    manager.update();

    replayGame data;
    static size_t currentFrame = 0;
    if (resetReplay) {
        player.getComponent<TransformComponent>().position.x = 256;
        player.getComponent<TransformComponent>().position.y = 132;
        tiles.clear();
        mapFile = "assets/map_tiles.png";
        Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);
        currentFrame = 0;
        resetReplay = false;
        replayFinished = false;
    }

    if (currentFrame < replayData.size()) {
        data = replayData[currentFrame++];
        player.getComponent<TransformComponent>().velocity.x = data.velocityX;
        player.getComponent<TransformComponent>().velocity.y = data.velocityY;
        player.getComponent<TransformComponent>().speed = data.speed;
    }
    else {
        replayFinished = true;
    }
    if (data.changeMap == 1) {
        tiles.clear();
        Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);
    }
    else if (data.changeMap == 2) {
        tiles.clear();
        mapFile = "assets/lab_tiles.png";
        Map::LoadMap("assets/lab.txt", Map::sizeX, Map::sizeY);
        player.getComponent<TransformComponent>().position.x = 256;
        player.getComponent<TransformComponent>().position.y = 132;
    }
    else if (data.changeMap == 3) {
        tiles.clear();
        mapFile = "assets/lab_tiles.png";
        Map::LoadMap("assets/level2.txt", Map::sizeX, Map::sizeY);
        player.getComponent<TransformComponent>().position.x = 940;
        player.getComponent<TransformComponent>().position.y = 1130;
    }

    if (player.getComponent<TransformComponent>().velocity.x > 0) {
        player.getComponent<SpriteComponent>().spriteFlip =SDL_FLIP_NONE;
        player.getComponent<SpriteComponent>().play("walk");
    } else if (player.getComponent<TransformComponent>().velocity.x < 0) {
        player.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
        player.getComponent<SpriteComponent>().play("walk");
    } else if (player.getComponent<TransformComponent>().velocity.y > 0) {
        player.getComponent<SpriteComponent>().play("walk");
    } else if (player.getComponent<TransformComponent>().velocity.y < 0) {
        player.getComponent<SpriteComponent>().play("walk");
    } else {
        player.getComponent<SpriteComponent>().play("idle");
    }
    Collision::CheckCollisions(player , tiles);
    GameLogic::CameraSystem(player);
}
void Game::renderReplay(){
    SDL_RenderClear(renderer);

    for (auto& t : tiles) t->draw();
    for (auto& p : players) p->draw();
    Texture::RenderText("PRESS R TO RESTART", 10, 10, 1);
    if (replayFinished) {
        Texture::RenderText("REPLAY FINISHED", 150, SCREEN_HEIGHT / 2, 3);
    }
    SDL_RenderPresent(renderer);
}

void Game::resetGame() {

    tiles.clear();
    enemies.clear();
    pets.clear();

    for (auto& e : enemies) e->removeAllComponentsAndGroups();
    for (auto& p : pets) p->removeAllComponentsAndGroups();

    inDungeon = false;
    level1Cleared = false;
    level2Cleared = false;
    game_over = false;
    score = 0;
    highscoreUpdated = false;

    mapFile = "assets/map_tiles.png";
    Map::LoadMap("assets/mapa.txt", Map::sizeX, Map::sizeY);

    player.getComponent<TransformComponent>().position.x = 256;
    player.getComponent<TransformComponent>().position.y = 132;
    player.getComponent<HPComponent>().healthPoints = 3;
    player.addGroup(groupPlayers);

    lab.getComponent<TransformComponent>().position.x = rand()%(500)+1000;
    lab.getComponent<TransformComponent>().position.y = rand()%(500)+1000;
    lab.addGroup(groupMap);
}

void Game::highscore(std::string username, int score) {
    std::cout<<"called"<<std::endl;
    users user;
    strcpy(user.name, username.c_str());
    user.score = score;

    std::ifstream highscoreFile("highscores.bin", std::ios::binary);
    std::ofstream newFile("new.bin", std::ios::binary);
    users other;
    bool check = false;
    while (highscoreFile.read((char*)&other, sizeof(other))) {
        if (user.score > other.score && !check) {
            newFile.write((char*)&user, sizeof(user));
            check = true;
        }
        newFile.write((char*)&other, sizeof(other));
    }
    if (!check) {
        newFile.write((char*)&user, sizeof(user));
    }
    newFile.close();
    highscoreFile.close();
    remove("highscores.bin");
    rename("new.bin", "highscores.bin");
}

void Game::outputScores() {
    users user;
    std::ifstream data("highscores.bin", std::ios::binary);
    if (data.is_open()) {
        while (data.read((char*)&user, sizeof(user))) {
            std::cout<<user.name<< " " << user.score << std::endl;
        }
    }
    data.close();
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

