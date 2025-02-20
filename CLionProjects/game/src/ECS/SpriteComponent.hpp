
#include"SDL.h"
#include"../Texture.hpp"
#include"Animation.hpp"
#include<map>
#include<exception>
#ifndef SPRITE_HPP
#define SPRITE_HPP

class SpriteComponent : public Component{
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames;
    int speed;

public:
    int animIndex=0;

  std::map<const char*, Animation> animations;

  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent()= default;
    SpriteComponent(const char* path){
      setTex(path);
    }
    ~SpriteComponent() {
      SDL_DestroyTexture(texture);
    }

    SpriteComponent(const char* path, bool isAnimated){
      animated = isAnimated;

      Animation idle = Animation(0, 2, 400);
      Animation walk = Animation(1, 6, 70);

      animations.emplace("idle", idle);
      animations.emplace("walk", walk);

      play("idle");
      play("walk");
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
      try {
        if (frames == 0)
          throw "deljenje z 0";
      }
      catch(const char* izjema) {
        std::cout<<izjema<<std::endl;
      }
      if (animated) {
        srcRect.x = srcRect.w * (int)((SDL_GetTicks() / speed) % frames);
        srcRect.y = animIndex * transform->height;
      }

        destRect.x = (int)transform->position.x - Game :: camera.x;
        destRect.y = (int)transform->position.y - Game :: camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;

    }
    void draw() override{
      Texture :: Draw(texture, srcRect, destRect, spriteFlip);
    }
    void play(const char* animName) {
      frames = animations[animName].frames;
      animIndex = animations[animName].index;
      speed = animations[animName].speed;
    }

};
#endif //SPRITE_HPP
