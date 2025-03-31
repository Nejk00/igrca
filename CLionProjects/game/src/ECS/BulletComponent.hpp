

#ifndef BULLETCOMPONENT_HPP
#define BULLETCOMPONENT_HPP
#include"Keyboard.hpp"


class BulletComponent : public Component {
    int xpos, ypos;
    TransformComponent* transform;
    public:

    BulletComponent(int posX, int posY) {
        xpos = posX;
        ypos = posY;
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
            float bulletVelX = xpos - entity->getComponent<TransformComponent>().position.x + Game::camera.x;
            float bulletVelY = ypos - entity->getComponent<TransformComponent>().position.y + Game::camera.y;

            std::cout<<xpos<<" "<<ypos<<std::endl;
            // Normalize direction to ensure constant speed
            float magnitude = sqrt(bulletVelX * bulletVelX + bulletVelY * bulletVelY);
            if (magnitude > 0) {
                bulletVelX /= magnitude;
                bulletVelY /= magnitude;
            }

            // Scale the speed (adjust speed as needed)
            float bulletSpeed = 10;
            bulletVelX *= bulletSpeed;
            bulletVelY *= bulletSpeed;

            entity->getComponent<TransformComponent>().velocity.x += bulletVelX;
            entity->getComponent<TransformComponent>().velocity.y += bulletVelY;
        }
    void update() override {
        if (entity->isActive()) {
        }
        else {
            transform->position.x = 5000;
            transform->position.y = 5000;
            transform->velocity.x = 0;
            transform->velocity.y = 0;
        }
    }
};
#endif //BULLETCOMPONENT_HPP
