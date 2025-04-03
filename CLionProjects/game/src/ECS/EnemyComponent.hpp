
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
    std::string type;

    EnemyComponent(Entity* target, std::string object) {
        playerTransform = &target->getComponent<TransformComponent>();
        type = object;
    }
    EnemyComponent() {}

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        targetVelocity.Zero();
    }

    void update() override {
        float directionX = playerTransform->position.x - transform->position.x;
        float directionY = playerTransform->position.y - transform->position.y;

        if (!chase) {
            transform->velocity.x = 0;
            transform->velocity.y = 0;
        }
        else{
            //float directionX = playerTransform->position.x - transform->position.x;
            //float directionY = playerTransform->position.y - transform->position.y;

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

        }
        if (type == "enemy") {
            if (!chase) {
                sprite->play("enemy_idle");
            }
            else if (directionX > 0){
                sprite->spriteFlip = SDL_FLIP_NONE;
                sprite->play("enemy_walk");
            }
            else if (directionX < 0){
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                sprite->play("enemy_walk");
            }
        }
        else if (type == "wolf") {
            if (!chase) {
                sprite->play("wolf_idle");
            }
            else if (directionX > 0) {
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                sprite->play("wolf_enemy");
            }
            else if(directionX < 0){
                sprite->spriteFlip = SDL_FLIP_NONE;
                sprite->play("wolf_enemy");
            }
        }
    }



};

#endif //AICOMPONENT_HPP
