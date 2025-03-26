

#ifndef MAP_HPP
#define MAP_HPP
#include <string>
#include<string>

class Map {
    public:
    static int sizeX, sizeY;
    Map();
    ~Map();
    static void LoadMap(std :: string path, int sizeX, int sizeY);

    private:


};

#endif //MAP_HPP
