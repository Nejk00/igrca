
#ifndef TURRETCOMPONENT_HPP
#define TURRETCOMPONENT_HPP

#include"../game.hpp"

class TurretComponent : public Component {
public:
    int range = 300;
    bool sight = false;
    int timeSinceLastBullet = 0;

    float posX = 0, posY = 0;

    float distanceSquared = 0;
    float dx = 0, dy = 0;
    TransformComponent* transform = nullptr;
    TransformComponent* targetTransform = nullptr;

    TurretComponent(Entity* target) {
        if (target) {
            targetTransform = &target->getComponent<TransformComponent>();
        }
    }

    TurretComponent() = default;
    ~TurretComponent() = default;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        if (!transform || !targetTransform) return;

        posX = targetTransform->position.x;
        posY = targetTransform->position.y;

        dx = transform->position.x - posX;
        dy = transform->position.y - posY;
        distanceSquared = dx * dx + dy * dy;

        sight = distanceSquared < range * range;

        /*if (timeSinceLastBullet > 150 && sight) {
            Game::addBullet(entity, 100, 100);
            timeSinceLastBullet = 0;
        }*/

        timeSinceLastBullet += Clock::delta;
    }
};

#endif //TURRETCOMPONENT_HPP