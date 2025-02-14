#ifndef COLIDERCOMPONENT_HPP
#define COLIDERCOMPONENT_HPP

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
        collider.w = transform->width;
        collider.h = transform->height;
    }
};

#endif //COLIDERCOMPONENT_HPP
