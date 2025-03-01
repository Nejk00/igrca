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

        if (!(!SDL_KEYUP == keystates[SDL_SCANCODE_SPACE])) {
            sprite->play("attack");
        }

        transform->velocity.x = velX;
        transform->velocity.y = velY;

        /*bool isAttacking = false;
        Uint32 attackStartTime = 0;
        const Uint32 attackDuration = 1200; // Adjust to match the attack animation duration

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                if (!isAttacking) { // Start attack only if not already attacking
                    isAttacking = true;
                    attackStartTime = SDL_GetTicks(); // Save attack start time
                    sprite->play("attack"); // Play attack animation
                }
            }
        }

        // Ensure attack state remains active until the animation completes
        if (isAttacking) {
            Uint32 elapsedTime = SDL_GetTicks() - attackStartTime;
            if (elapsedTime >= attackDuration) {
                isAttacking = false; // Reset attack state
                //sprite->play("idle"); // Return to idle or another appropriate animation
            }
        }*/
    }


};

#endif //KEYBOARD_HPP
