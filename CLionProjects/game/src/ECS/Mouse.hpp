
#ifndef MOUSE_HPP
#define MOUSE_HPP

#include"../game.hpp"

class Mouse : public Component {
    public:
    bool clicked = false;
    int mouseX, mouseY;
    int timeSinceLastBullet = 1e9;

    void init() override {

    }

    void update() override {
        SDL_Event event;

        SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&event)) {
            if (SDL_MOUSEBUTTONDOWN == event.type) {
                if (SDL_BUTTON_LEFT == event.button.button) {
                    clicked = true;
                }
            }
            if(SDL_MOUSEBUTTONUP == event.type){
                if(SDL_BUTTON_LEFT == event.button.button){
                    clicked = false;
                }
            }
            if (clicked && timeSinceLastBullet > 100) {
                Game::addBullet(entity, mouseX, mouseY);
                timeSinceLastBullet = 0;
            }
            if (SDL_QUIT) {

            }
            timeSinceLastBullet += Clock :: delta;
        }

    }
};

#endif //MOUSE_HPP
