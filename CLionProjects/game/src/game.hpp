

#ifndef GAME_HPP
#define GAME_HPP

#include"SDL2/SDL.h"
#include<stdio.h>
#include<SDL_image.h>
#include<vector>

class ColiderComponent;

class Game {
    bool isRunning;
    SDL_Window *window;

    public:
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColiderComponent*> coliders;
    Game();
    ~Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    static void AddTile(int id, int x, int y);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running(){
        return isRunning;
    }


};
#endif //GAME_HPP
