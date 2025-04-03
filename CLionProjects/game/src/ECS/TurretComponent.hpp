
#ifndef TURRETCOMPONENT_HPP
#define TURRETCOMPONENT_HPP

#include"../game.hpp"

class TurretComponent : public Component {
public:
    int range = 300;
    bool sight = false;
    int timeSinceLastBullet = 1e9;

    float posX = 0, posY =0;

    float distanceSquared = 0;
    float dx = 0, dy = 0;
    TransformComponent* transform;
    TransformComponent* targetTransform;
    TransformComponent tmpTransform;

    TurretComponent(Entity* target){
        targetTransform = &target->getComponent<TransformComponent>();
    }
    TurretComponent() = default;
    ~TurretComponent() = default;


    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        if (targetTransform) {
            tmpTransform = *targetTransform;
        }
        posX = tmpTransform.position.x;
        posY = tmpTransform.position.y;

        dx = transform->position.x - tmpTransform.position.x;
        dy = transform->position.y - tmpTransform.position.y;
        distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < range * range)
            sight = true;
        else
            sight = false;

        if (timeSinceLastBullet > 150 && sight) {
            Game::addBullet(entity, (int)posX, (int)posY);
            timeSinceLastBullet = 0;
        }
        timeSinceLastBullet += Clock :: delta;
    }
};

#endif //TURRETCOMPONENT_HPP