
#include"game.hpp"
#ifndef MAP_HPP
#define MAP_HPP

class Map {
    public:
    Map();
    ~Map();


    void LoadMap(int arr[20][20]);
    void DrawMap();

    private:

    SDL_Rect src, dest;
    SDL_Texture *dirt;
    SDL_Texture *grass;
    SDL_Texture *water;

    int map[20][20];

};

#endif //MAP_HPP
