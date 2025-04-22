

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

    static bool replayFinished;
    static bool resetReplay;
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

    SDL_Texture *mainmenuscreen;
    SDL_Texture *background;


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


    void saveGame();
    void loadGame();

    void saveReplay();
    void highscore(std::string username, int score);
    void outputScores();
    void calculateScore();

    void initReplay();
    void replay();
    void renderReplay();

    void renderMainMenu();
    void renderPause();
    void renderGameOver();
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
