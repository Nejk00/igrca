

#ifndef BULLETCOMPONENT_HPP
#define BULLETCOMPONENT_HPP
#include"Keyboard.hpp"


class BulletComponent : public Component {
    float xpos, ypos;
    float speed = 10;
    TransformComponent* transform;
    SpriteComponent* sprite;
    public:

    BulletComponent(int posX, int posY) {
        xpos = posX;
        ypos = posY;
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
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
            bulletVelX *= speed;
            bulletVelY *= speed;

            transform->velocity.x += bulletVelX;
            transform->velocity.y += bulletVelY;

        }
    void update() override {
        //sprite->play("bullet");
    }
};
#endif //BULLETCOMPONENT_HPP
