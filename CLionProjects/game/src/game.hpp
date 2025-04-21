

#ifndef GAME_HPP
#define GAME_HPP

#include"SDL2/SDL.h"
#include<stdio.h>
#include<SDL_image.h>
#include<vector>
#include"ECS/ECS.hpp"
#include<fstream>
#include"Clock.hpp"


class ColiderComponent;
class EnemyCollider;

class Game {
    SDL_Window *window;

public:
    static bool highscoreUpdated;
    static int flags;
    std::string username;
    std::ifstream replayFile;
    static int score;
    static float scaleX, scaleY;
    bool isRunning;
    static bool game_over;
    static float prevX, prevY;
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
    static int LETTER_WIDTH, LETTER_HEIGHT;




    SDL_Texture *bulletTexture;
    SDL_Texture *hpTexture;
    SDL_Texture *savedPet;

    SDL_Texture *game_overTexture;
    SDL_Texture *winTexture;
    SDL_Texture *mainmenuscreen;
    SDL_Texture *pauseMenu;
    SDL_Texture *options;

    SDL_Texture *button_options;
    SDL_Texture *button_exit;
    SDL_Texture *button_play;
    SDL_Texture *button_small;
    SDL_Texture *button_large;
    SDL_Texture *button_medium;
    SDL_Texture *button_resume;
    SDL_Texture *button_main_menu;
    SDL_Texture *button_back_to_menu;
    SDL_Texture *button_save_game;
    SDL_Texture *button_load_game;
    SDL_Texture *button_replay;
    SDL_Texture *button_play_again;

    static SDL_Texture *numbersTexture;
    static SDL_Texture *lettersTexture;

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
    void centerWindow(SDL_Window*);
    void RenderNumber(int number, int x, int y, float scale);
    void calculateScore();

    void saveGame();
    void loadGame();
    void highscore(std::string username, int score);
    void outputScores();

    void initReplay();
    void replay();
    void renderReplay();


    void initMainMenu();
    void updateMainMenu();
    void renderMainMenu();

    void initOptions();
    void renderOptions();
    void updateOptions();

    void initPause();
    void renderPause();
    void updatePause();

    void initGameOver();
    void updateGameOver();
    void renderGameOver();

    void initUserInput();
    void updateUserInput();
    void renderUserInput();

    void run();
    void update();
    void render();
    void clean();
    void resetGame();

    bool running(){
        return isRunning;
    }


};
#endif //GAME_HPP
