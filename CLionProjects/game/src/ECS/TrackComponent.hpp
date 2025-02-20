#include"../game.hpp"
#include<cmath>
#include "ColiderComponent.hpp"

#ifndef TRACKCOMPONENT_HPP
#define TRACKCOMPONENT_HPP

class TrackComponent : public Component{
    int speed;
    Entity* player;
    TransformComponent* transform;
    SpriteComponent* sprite;


    public:

    TrackComponent(int sp) {
        speed = sp;
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override{
        if (player->getComponent<TransformComponent>().position.x > transform->position.x)
                transform->velocity.x += speed;
        else if (player->getComponent<TransformComponent>().position.x < -transform->position.x)
                transform->velocity.x -= speed;
        if (player->getComponent<TransformComponent>().position.y > transform->position.y)
                transform->velocity.y += speed;
        else if (player->getComponent<TransformComponent>().position.y < -transform->position.y)
                transform->velocity.y -= speed;
        
    }
};


#endif //TRACKCOMPONENT_HPP
