#include"../game.hpp"
#include<cmath>
#include<iostream>


#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

class Keyboard : public Component {
    public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }
    void update() override {
        const Uint8* keystates = SDL_GetKeyboardState(nullptr);

        float velX = (keystates[SDL_SCANCODE_A] ? -1.0f : 0.0f) +(keystates[SDL_SCANCODE_D] ? 1.0f : 0.0f);
        float velY = (keystates[SDL_SCANCODE_W] ? -1.0f : 0.0f) +(keystates[SDL_SCANCODE_S] ? 1.0f : 0.0f);

        // Compute vector length
        float length = std::sqrt(velX * velX + velY * velY);

        // Normalize if necessary
        if (length > 0) {
            velX /= length;
            velY /= length;
        }
        if (keystates[SDL_SCANCODE_F1]) {
            Game::game_over = true;
        }

        if (keystates[SDL_SCANCODE_LSHIFT]) {
            transform->speed = 4;
        }
        else {
            transform->speed = 3;
        }

        if (velX > 0) {
            sprite->spriteFlip =SDL_FLIP_NONE;
            sprite->play("walk");
        } else if (velX < 0) {
            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
            sprite->play("walk");
        } else if (velY > 0) {
            sprite->play("walk");
        } else if (velY < 0) {
            sprite->play("walk");
        } else {
            sprite->play("idle"); // If no movement, set idle animation
        }

        if (!entity->getComponent<TransformComponent>().isKnockbacked) {
            transform->velocity.x = velX;
            transform->velocity.y = velY;
        }
    }


};

#endif //KEYBOARD_HPP
