

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
    static float scaleX, scaleY;
    bool isRunning;
    static bool level1Cleared;
    static bool level2Cleared;
    static bool clicked;
    static bool inDungeon;
    static SDL_Rect camera;
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColiderComponent*> colliders;
    static int SCREEN_HEIGHT;
    static int SCREEN_WIDTH;
    static int all_pets;




    SDL_Texture *bulletTexture;
    SDL_Texture *hpTexture;
    SDL_Texture *savedPet;

    SDL_Texture *game_overTexture;
    SDL_Texture *winTexture;
    SDL_Texture *mainmenuscreen;
    SDL_Texture *pauseMenu;

    SDL_Texture *button_options;
    SDL_Texture *button_exit;
    SDL_Texture *button_play;
    SDL_Texture *button_small;
    SDL_Texture *button_large;
    SDL_Texture *button_medium;
    SDL_Texture *button_resume;
    SDL_Texture *button_main_menu;

    Game() = default;
    ~Game() = default;

    void init(const char* title, int xpos, int ypos, bool fullscreen);
    static void AddTile(int srcX, int srcY, int xpos, int ypos, bool hasCollision);
    static void addBullet(Entity*, float targetX, float targetY);
    static void CameraSystem();
    static void changeMap(float, float);
    static void loadDungeon();
    static void backToIsland( float, float);
    static void Dungeon();
    void handleEvents();

    void initMainMenu();
    void updateMainMenu();
    void renderMainMenu();

    void initOptions();
    void renderOptions();
    void updateOptions();

    void initPause();
    void renderPause();
    void updatePause();

    void run();
    void update();
    void render();
    void clean();

    bool running(){
        return isRunning;
    }


};
#endif //GAME_HPP
