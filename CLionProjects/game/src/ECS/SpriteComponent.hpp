
#include"SDL.h"
#include"../Texture.hpp"
#include"Animation.hpp"
#include<map>
#include<string>
#include<ctime>
#include<exception>
#ifndef SPRITE_HPP
#define SPRITE_HPP

class SpriteComponent : public Component{
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames = 1;
    int speed = 1;

    std::string tag;
    std::string animation;

public:
    const char *Texture;
    int animIndex=0;

  std::map<const char*, Animation> animations;

  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent()= default;
    SpriteComponent(const char* path){
      Texture = path;
      setTex(path);
    }
  SpriteComponent(bool random, const char * tag) {
      srand(time(NULL));
      if (random && std::string(tag) == "pet") {
        if (rand()%2 == 0) {
          setTex("assets/pet.png");
        }
        else {
          setTex("assets/pet2.png");
        }
      }
      else if (random && std::string(tag) == "enemy") {
        /*if (rand()%2 == 0) */{
          setTex("assets/enemy.png");
        }
      }
    }
    ~SpriteComponent() {
      SDL_DestroyTexture(texture);
    }

    SpriteComponent(const char* path, bool isAnimated){
      animated = isAnimated;

      Animation idle = Animation(0, 2, 400);
      Animation walk = Animation(1, 8, 100);
      //Animation attack = Animation(8, 8, 100);

      Animation pet_idle = Animation(0, 2, 400);
      Animation pet_walk = Animation(1, 6, 100);

      Animation enemy_idle = Animation(0, 4, 100);
      Animation enemy_walk = Animation(1, 4, 100);

      Animation wolf_enemy = Animation(0, 5, 100);
      Animation wolf_idle = Animation(1, 6, 150);

      Animation bullet = Animation(0, 2, 150);

      animations.emplace("idle", idle);
      animations.emplace("walk", walk);


      animations.emplace("pet_idle", pet_idle);
      animations.emplace("pet_walk", pet_walk);

      animations.emplace("enemy_idle", enemy_idle);
      animations.emplace("enemy_walk", enemy_walk);

      animations.emplace("wolf_enemy", wolf_enemy);
      animations.emplace("wolf_idle", wolf_idle);

      animations.emplace("bullet", bullet);

      play("idle");
      play("walk");
      play("attack");

      play("pet_idle");
      play("pet_walk");

      play("enemy_idle");
      play("enemy_walk");

      play("wolf_enemy");
      play("wolf_idle");

      play("bullet");

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
        destRect.w = transform->width * transform->scale /** Game::scaleX*/;
        destRect.h = transform->height * transform->scale /** Game::scaleY*/;

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
