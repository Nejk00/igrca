
#ifndef BUTTONCOMPONENT_HPP
#define BUTTONCOMPONENT_HPP

#include"../Texture.hpp"

class buttonComponent : public Component {
    public:
    SDL_Rect srcRect, destRect {0, 0, 150, 70};
    SDL_Texture* texture;

    buttonComponent(int xpos, int ypos, int width, int height, const char* path) {
        srcRect.x = srcRect.y = 0;
        srcRect.w = width;
        srcRect.h = height;
        destRect.x = xpos;
        destRect.y = ypos;
        texture = Texture::LoadTexture(path);
    }
    void init() override {

    }
    void update() override {

    }
};

#endif //BUTTONCOMPONENT_HPP
