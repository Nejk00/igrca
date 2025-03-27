
#ifndef TURRETCOMPONENT_HPP
#define TURRETCOMPONENT_HPP

#include"../game.hpp"

class TurretComponent : public Component {
    public:
    int range = 300;
    bool sight = false;
    int timeSinceLastBullet = 1e9;
    int posX = 0, posY = 0;
    TransformComponent* targetTransform;
    float distanceSquared = 0;
    float dx = 0, dy = 0;
    TransformComponent* transform;

   TurretComponent(Entity& target){
       posX = target.getComponent<TransformComponent>().position.x;
       posY = target.getComponent<TransformComponent>().position.y;
       targetTransform = &target.getComponent<TransformComponent>();
    }
    TurretComponent() = default;
    ~TurretComponent() = default;


    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        dx = transform->position.x - targetTransform->position.x;
        dy = transform->position.y - targetTransform->position.y;
        distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < range * range)
            sight = true;
        else
            sight = false;

        if (timeSinceLastBullet > 150 && sight) {
            Game::addBullet(entity, posX, posY);
            timeSinceLastBullet = 0;
        }
        timeSinceLastBullet += Clock :: delta;
        std::cout << posX << ", " << posY << std::endl;
    }
};

#endif //TURRETCOMPONENT_HPP
