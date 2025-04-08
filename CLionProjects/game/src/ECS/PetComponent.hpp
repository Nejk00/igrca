
#ifndef PETCOMPONENT_HPP
#define PETCOMPONENT_HPP

#include"../Collision.hpp"


class PetComponent : public Component {
public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    SpriteComponent* sprite;
    Vector2D targetVelocity;
    Entity* target;

    bool follow = false;
    bool saved = false;

    PetComponent(Entity* target) {
        playerTransform = &target->getComponent<TransformComponent>();
    }
    PetComponent() {}

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        targetVelocity.Zero();
    }

    void update() override {
            float directionX = playerTransform->position.x - transform->position.x;
            float directionY = playerTransform->position.y - transform->position.y;

            // Normalize the direction vector
            float length = std::sqrt(directionX * directionX + directionY * directionY);
            if (length > 0) {
                directionX /= length;
                directionY /= length;
            }

            // Smoothly interpolate the velocity
            float lerpFactor = 0.1f;
            targetVelocity.x = directionX * transform->speed;
            targetVelocity.y = directionY * transform->speed;


            if (follow){
                transform->velocity.x += (targetVelocity.x - transform->velocity.x) * lerpFactor;
                transform->velocity.y += (targetVelocity.y - transform->velocity.y) * lerpFactor;
            }
            else {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
            }
            if (follow == false) {
                sprite->play("pet_idle");
            }
            else if (directionX > 0){
                sprite->spriteFlip = SDL_FLIP_NONE;
                sprite->play("pet_walk");
            }
            else{
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                sprite->play("pet_walk");
            }
        }

};

#endif //PETCOMPONENT_HPP BOSTI KURAC
