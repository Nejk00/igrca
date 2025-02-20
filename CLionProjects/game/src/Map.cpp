#include"Map.hpp"
#include <ctime>
#include <fstream>
#include <bits/locale_classes.h>
#include"game.hpp"
#include<fstream>



Map::~Map() {

}


Map::Map() {


}

void Map::LoadMap(std::string path, int sizeX, int sizeY) {
    int tile;
    std::fstream mapFile;
    mapFile.open(path);
    bool hasCollision;
    int srcX, srcY;

    for (int y = 0; y < sizeY; y++)
        for (int x = 0; x < sizeX; x++) {
            mapFile>>tile;
            srcY = (tile/10)*32;
            srcX = (tile % 10)*32;
            hasCollision = (tile == 0);
            Game::AddTile(srcX, srcY, x * 32, y * 32, hasCollision);
        }
    mapFile.close();
}










