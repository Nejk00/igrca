#include"GameLogic.hpp"
#include <vector>
#include"Collision.hpp"

void GameLogic :: enemyMovement(Entity& player, std::vector<Entity*>& enemies) {
    for (auto& e : enemies) {
        int range = 200;
        float trgtVelocityX;
        float trgtVelocityY;
        auto& enemyTransform = e->getComponent<TransformComponent>();
        auto& playerTransform = player.getComponent<TransformComponent>();

        // Calculate the direction vector
        float directionX = playerTransform.position.x - enemyTransform.position.x;
        float directionY = playerTransform.position.y - enemyTransform.position.y;

        // Compute the distance
        float distance = std::sqrt(directionX * directionX + directionY * directionY);

        // Check if the enemy is within range
        if (distance <= range) {
            // Normalize the direction vector
            if (distance > 0) {
                directionX /= distance;
                directionY /= distance;
            }

            // Calculate target velocity
            trgtVelocityX = directionX * enemyTransform.speed;
            trgtVelocityY = directionY * enemyTransform.speed;
        }
        enemyTransform.velocity.x += (trgtVelocityX - enemyTransform.velocity.x);
        enemyTransform.velocity.y += (trgtVelocityY - enemyTransform.velocity.y);

        if (distance > range) {
            e->getComponent<SpriteComponent>().play("enemy_idle");
        }
        else if (directionX > 0){
            e->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
            e->getComponent<SpriteComponent>().play("enemy_walk");
        }
        else{
            e->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
            e->getComponent<SpriteComponent>().play("enemy_walk");
        }
    }
}

void GameLogic :: petMovement(Entity& player, std::vector<Entity*>& pets) {
    for (auto& p : pets) {
        static bool follow = false;
        //auto& petTransform = p->getComponent<TransformComponent>();
        auto& playerTransform = player.getComponent<TransformComponent>();

        // Calculate the direction vector
        float directionX = playerTransform.position.x - p->getComponent<TransformComponent>().position.x;
        float directionY = playerTransform.position.y - p->getComponent<TransformComponent>().position.y;

        // Normalize the direction vector
        float length = std::sqrt(directionX * directionX + directionY * directionY);
        if (length > 0) {
            directionX /= length;
            directionY /= length;
        }

        // Smoothly interpolate the velocity
        float lerpFactor = 0.05f;
        float targetVelocityX = directionX * p->getComponent<TransformComponent>().speed;
        float targetVelocityY = directionY * p->getComponent<TransformComponent>().speed;

        const Uint8* keystates = SDL_GetKeyboardState(nullptr);

        if (Collision::AABB(player.getComponent<ColiderComponent>(), p->getComponent<ColiderComponent>())){
            if (keystates[SDL_SCANCODE_F]) {
                follow = !follow;
            }
                targetVelocityX = 0;
                targetVelocityY = 0;
            }

            if (follow)
            {
                p->getComponent<TransformComponent>().velocity.x += (targetVelocityX - p->getComponent<TransformComponent>().velocity.x) * lerpFactor;
                p->getComponent<TransformComponent>().velocity.y += (targetVelocityY - p->getComponent<TransformComponent>().velocity.y) * lerpFactor;
            }
            if (follow == false) {
                p->getComponent<SpriteComponent>().play("pet_idle");
            }
            else if (Collision::AABB(player.getComponent<ColiderComponent>(), p->getComponent<ColiderComponent>())){
                p->getComponent<SpriteComponent>().play("pet_idle");
            }
            else if (directionX > 0){
                p->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
                p->getComponent<SpriteComponent>().play("pet_walk");
            }
            else{
                p->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
                p->getComponent<SpriteComponent>().play("pet_walk");
            }
        }
    }


void GameLogic :: CameraSystem(Entity& player) {
    Game::camera.x = player.getComponent<TransformComponent>().position.x - 640/2;
    Game::camera.y = player.getComponent<TransformComponent>().position.y - 640/2;

    if (Game::camera.x < 0) Game::camera.x = 0;
    if (Game::camera.y < 0) Game::camera.y = 0;//hvala
    if (Game::camera.x + 640 > 80*32) Game::camera.x = 80*32 - 640;
    if (Game::camera.y + 640 > 80*32) Game::camera.y = 80*32 - 640;
}