
#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP

#include"ECS.hpp"
#include"SDL.h"

class TileComponent : public Component {
    public:

    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    Vector2D position;
    TransformComponent* transform;


    TileComponent()= default;

    ~TileComponent() {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xpos, int ypos, std::string path) {
        texture = Texture::LoadTexture(path.c_str());

        position.x = xpos;
        position.y = ypos;

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = 32;

        destRect.x = xpos;
        destRect.y = ypos;
        destRect.w = destRect.h = 32;
    }
    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }
    void update() override {
        destRect.x = position.x - Game :: camera.x;
        destRect.y = position.y - Game :: camera.y;
        //destRect.w = transform->width * Game::scaleX;
        //destRect.h = transform->height * Game::scaleY;
    }
    void draw() override {
        Texture::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }



};


#endif //TILECOMPONENT_HPP
