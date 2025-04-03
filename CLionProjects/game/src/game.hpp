

#ifndef GAME_HPP
#define GAME_HPP

#include"SDL2/SDL.h"
#include<stdio.h>
#include<SDL_image.h>
#include<vector>
#include"ECS/ECS.hpp"
#include"Clock.hpp"


class ColiderComponent;
class EnemyCollider;

class Game {
    SDL_Window *window;

public:
    bool isRunning;
    static bool inDungeon;
    static SDL_Rect camera;
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColiderComponent*> colliders;
    static const int SCREEN_HEIGHT;
    static const int SCREEN_WIDTH;
    static int all_pets;

    SDL_Texture *bulletTexture;
    SDL_Texture *hpTexture;
    SDL_Texture *missingHpTexture;
    SDL_Texture *game_overTexture;
    SDL_Texture *winTexture;
    SDL_Texture *savedPet;

    Game() = default;
    ~Game() = default;
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    static void AddTile(int srcX, int srcY, int xpos, int ypos, bool hasCollision);
    static void addBullet(Entity*, int targetX, int targetY);
    static void CameraSystem();
    static void changeMap(float, float);
    static void Dungeon();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running(){
        return isRunning;
    }


};
#endif //GAME_HPP
