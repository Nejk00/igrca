#include"Map.hpp"

#include <ctime>
#include <bits/locale_classes.h>

#include"Texture.hpp"

int lvl1[20][20];/* = {
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

void Map::LoadMap(int arr[20][20]) {
    for(int row=0; row<20; row++)
        for(int clmn=0; clmn<20; clmn++) {
            map[row][clmn] = arr[row][clmn];
        }
}

void Map::DrawMap() {
    int type =0;

    for(int row=0; row<20; row++)
        for(int clmn=0; clmn<20; clmn++) {
            type = map[row][clmn];
            dest.x = clmn * 32;
            dest.y = row * 32;

            switch(type) {
                case 0:
                    Texture :: Draw(water, src, dest);
                    break;
                case 1:
                    Texture :: Draw(grass, src, dest);
                    break;
                case 2:
                    Texture :: Draw(dirt, src, dest);
                    break;
            }
        }

}