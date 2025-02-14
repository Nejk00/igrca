#include"../game.hpp"

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

class Keyboard : public Component {
    public:
    TransformComponent* transform;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }
    void update() override {
        if (Game::event.type == SDL_KEYDOWN) {
            if (Game::event.key.keysym.sym == SDLK_w) {
                transform->velocity.y = -1;
            }
            if (Game::event.key.keysym.sym == SDLK_s) {
                transform->velocity.y = 1;
            }
            if (Game::event.key.keysym.sym == SDLK_a) {
                transform->velocity.x = -1;
            }
            if (Game::event.key.keysym.sym == SDLK_d) {
                transform->velocity.x = 1;
            }
        }

        if (Game::event.type == SDL_KEYUP) {
            if (Game::event.key.keysym.sym == SDLK_w) {
                transform->velocity.y = 0;
            }
            if (Game::event.key.keysym.sym == SDLK_s) {
                transform->velocity.y = 0;
            }
            if (Game::event.key.keysym.sym == SDLK_a) {
                transform->velocity.x = 0;
            }
            if (Game::event.key.keysym.sym == SDLK_d) {
                transform->velocity.x = 0;
            }
        }
    }






};

#endif //KEYBOARD_HPP
