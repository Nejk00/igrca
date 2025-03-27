
#ifndef AICOMPONENT_HPP
#define AICOMPONENT_HPP
#include "Components.hpp"
#include"Components.hpp"
#include"ECS.hpp"

class EnemyComponent : public Component {
public:
    bool chase = true;
    TransformComponent* transform;
    TransformComponent* playerTransform;
    SpriteComponent* sprite;
    Vector2D targetVelocity;
    Entity* target;

    EnemyComponent(Entity* target) {
        playerTransform = &target->getComponent<TransformComponent>();
    }
    EnemyComponent() {}

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        targetVelocity.Zero();
    }

    void update() override {
        if (chase){
            float directionX = playerTransform->position.x - transform->position.x;
            float directionY = playerTransform->position.y - transform->position.y;

            float distance = std::sqrt(directionX * directionX + directionY * directionY);

            // Normalize the direction vector
            if (distance > 0) {
                directionX /= distance;
                directionY /= distance;
            }

            // Calculate target velocity
            targetVelocity.x = directionX * transform->speed;
            targetVelocity.y = directionY * transform->speed;

            transform->velocity.x += (targetVelocity.x - transform->velocity.x);
            transform->velocity.y += (targetVelocity.y - transform->velocity.y);


            if (directionX > 0){
                sprite->spriteFlip = SDL_FLIP_NONE;
                sprite->play("enemy_walk");
            }
            else{
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                sprite->play("enemy_walk");
            }
        }
    }

};

#endif //AICOMPONENT_HPP
