

#ifndef GAME_HPP
#define GAME_HPP

#include"SDL2/SDL.h"
#include<stdio.h>
#include<SDL_image.h>
#include<vector>
#include"ECS/ECS.hpp"


class ColiderComponent;
class EnemyCollider;

class Game {
    bool isRunning;
    SDL_Window *window;

    public:
    static SDL_Rect camera;
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColiderComponent*> colliders;
    Game();
    ~Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    static void AddTile(int srcX, int srcY, int xpos, int ypos, bool hasCollision);
    static void addBullet(Entity*);
    static void CameraSystem();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running(){
        return isRunning;
    }


};
#endif //GAME_HPP
