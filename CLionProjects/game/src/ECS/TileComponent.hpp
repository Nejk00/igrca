
#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP

#include"ECS.hpp"
#include"TransformComponent.hpp"
#include"SpriteComponent.hpp"
#include"SDL.h"
#include<string>

class TileComponent : public Component {
    public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    SDL_Rect tileRect;
    int tileID;
    char* path;

    TileComponent()= default;

    TileComponent(int x, int y, int w, int h, int id) {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        switch (tileID) {
            case 0:
                path = "C:/Users/nejcg/CLionProjects/game/assets/water.png";
                break;
            case 1:
                path = "C:/Users/nejcg/CLionProjects/game/assets/grass.png";
                break;
            case 2:
                path = "C:/Users/nejcg/CLionProjects/game/assets/dirt.png";
                break;
            case 3:
                path = "C:/Users/nejcg/CLionProjects/game/assets/wall.png";
            default:
                break;

        }
    }
    void init() override {
        entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, (float)tileRect.w, (float)tileRect.h, 1);
        transform = &entity->getComponent<TransformComponent>();
        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }

};


#endif //TILECOMPONENT_HPP
