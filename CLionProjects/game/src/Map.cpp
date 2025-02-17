/*#include"Map.hpp"

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
    char tile;
    std::fstream mapFile;
    mapFile.open(path);
    for (int y = 0; y < sizeY; y++)
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(tile);
            Game::AddTile(atoi(&tile) , x*32, y*32);
            mapFile.ignore();
        }
    mapFile.close();
       }
   }
}*/


#include"Map.hpp"
#include<fstream>

#include <ctime>
#include <iostream>
#include <bits/locale_classes.h>

#include"Texture.hpp"

char lvl1[20][20]; /*= {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};*/

Map::~Map() {
    SDL_DestroyTexture(grass);
    SDL_DestroyTexture(water);
    SDL_DestroyTexture(dirt);
}


Map::Map() {
    srand(time(NULL));
    dirt=Texture :: LoadTexture("C:/Users/nejcg/CLionProjects/game/assets/dirt.png");
    grass=Texture :: LoadTexture("C:/Users/nejcg/CLionProjects/game/assets/grass.png");
    water=Texture :: LoadTexture("C:/Users/nejcg/CLionProjects/game/assets/water.png");

    for (int i=0;i<20;i++)
        for (int j=0;j<20;j++)
            lvl1[i][j]=0;

    LoadMap(lvl1);

    src.x =src.y = 0;
    src.w =dest.w =32;
    src.h =dest.h =32;
}

void Map::LoadMap(char arr[20][20]) {
    for(int row=0; row<20; row++)
        for(int clmn=0; clmn<20; clmn++) {
            map[row][clmn] = arr[row][clmn];
        }
}

void Map::DrawMap() {
    char type;
    std::fstream mapFile;
    mapFile.open("C:/Users/nejcg/CLionProjects/game/assets/map.txt");
    for (int y = 0; y < 20; y++)
        for (int x = 0; x < 20; x++) {
            mapFile >> type;
            std::cout << type<< std::endl;
            dest.x = x * 32;
            dest.y = y * 32;
            switch(type) {
                case '0':
                    Texture :: Draw(water, src, dest);
                break;
                case '1':
                    Texture :: Draw(grass, src, dest);
                break;
                case '2':
                    Texture :: Draw(dirt, src, dest);
                break;
            }

        }
    mapFile.close();
}



