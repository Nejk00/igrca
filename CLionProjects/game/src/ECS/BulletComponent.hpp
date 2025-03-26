

#ifndef BULLETCOMPONENT_HPP
#define BULLETCOMPONENT_HPP
#include"Keyboard.hpp"

class BulletComponent : public Component {
    int xpos, ypos;
    public:

    BulletComponent(int mouseX, int mouseY) {
        xpos = mouseX;
        ypos = mouseY;
    }

    void init() override {
        float bulletVelX = xpos - entity->getComponent<TransformComponent>().position.x + Game::camera.x;
        float bulletVelY = ypos - entity->getComponent<TransformComponent>().position.y + Game::camera.y;

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
};
#endif //BULLETCOMPONENT_HPP
