#ifndef COLIDERCOMPONENT_HPP
#define COLIDERCOMPONENT_HPP
#include<iostream>
#include<string>
#include"Components.hpp"
#include"SDL.h"



class ColiderComponent : public Component {
    public:
    SDL_Rect collider;
    std::string tag;

    TransformComponent* transform;

    ColiderComponent(std::string t) {
        tag = t;
    }

    void init() override {
        if (!entity->hasComponent<TransformComponent>())
            entity->addComponent<TransformComponent>();
        transform = &entity->getComponent<TransformComponent>();

        Game::coliders.push_back(this);

    }
    void update() override {
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
        std::cout<<"collider collider.x = "<<collider.x<<std::endl;
    }
    void draw() override {
        SDL_Rect tmp;
        tmp.x = collider.x - Game :: camera.x;
        tmp.y = collider.y - Game :: camera.y;
        tmp.w = collider.w;
        tmp.h = collider.h;
        SDL_SetRenderDrawColor(Game :: renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(Game :: renderer, &tmp);
    }
};

#endif //COLIDERCOMPONENT_HPP
