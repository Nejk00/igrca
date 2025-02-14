
#include"SDL.h"
#include"../Texture.hpp"
#ifndef SPRITE_HPP
#define SPRITE_HPP

class SpriteComponent : public Component{
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

    bool animated;
    int frames = 0;
    int speed = 100;

public:
    SpriteComponent()= default;
    SpriteComponent(const char* path){
      setTex(path);
    }
    ~SpriteComponent() {
      SDL_DestroyTexture(texture);
    }

    SpriteComponent(const char* path, int nFrames, int speed){
      animated = true;
      frames = nFrames;
      speed = speed;
      setTex(path);
    }
    void setTex(const char* path) {
      texture = Texture :: LoadTexture(path);
    }
    void init() override{
      transform = &entity->getComponent<TransformComponent>();

      srcRect.x = 0;
      srcRect.y = 0;
      srcRect.w = transform->width;
      srcRect.h = transform->height;
    }
    void update() override{
      if (animated){
        srcRect.x = srcRect.w * (int)((SDL_GetTicks() / speed) % frames);
      }
      destRect.x = (int)transform->position.x;
      destRect.y = (int)transform->position.y;
      destRect.w = transform->width * transform->scale;
      destRect.h = transform->height * transform->scale;
    }
    void draw() override{
      Texture :: Draw(texture, srcRect, destRect);
    }

};
#endif //SPRITE_HPP
