
/*
#ifndef MAP_HPP
#define MAP_HPP
#include <string>
#include<string>

class Map {
    public:
    Map();
    ~Map();
    static void LoadMap(std :: string path, int sizeX, int sizeY);

    private:


};

#endif //MAP_HPP*/
#include"game.hpp"
#ifndef MAP_HPP
#define MAP_HPP

class Map {
public:
    Map();
    ~Map();


    void LoadMap(char arr[20][20]);
    void DrawMap();

private:

    SDL_Rect src, dest;
    SDL_Texture *dirt;
    SDL_Texture *grass;
    SDL_Texture *water;

    int map[20][20];

};

#endif //MAP_HPP