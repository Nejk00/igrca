
#ifndef HPCOMPONENT_HPP
#define HPCOMPONENT_HPP
#include "PlayerComponent.hpp"


class HPComponent : public Component {
public:
    int healthPoints = 1;
    int maxHealthPoints = 3;
    HPComponent(int n) {
        healthPoints = n;
    }

    void init() override {

    }
    void update() override {
        if (healthPoints <= 0) {
            if (entity->hasComponent<PlayerComponent>()) {
                Game::game_over = true;
            }
            else{
                entity->deactivate();
            }
        }
    }
};

#endif //HPCOMPONENT_HPP
