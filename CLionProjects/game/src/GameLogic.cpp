#include"GameLogic.hpp"
#include"Collision.hpp"


static void enemyMovement(Entity& enemy, Entity& player) {
    auto& enemyTransform = enemy.getComponent<TransformComponent>();
    auto& playerTransform = player.getComponent<TransformComponent>();

    // Calculate the direction vector
    float directionX = playerTransform.position.x - enemyTransform.position.x;
    float directionY = playerTransform.position.y - enemyTransform.position.y;

    // Calculate the distance
    float distance = std::sqrt(directionX * directionX + directionY * directionY);
    float detectionRadius = 200.0f; // Enemy follows only within this radius

    // Normalize the direction vector
    if (distance > 0) {
        directionX /= distance;
        directionY /= distance;
    }

    // Enemy follows only if within range
    if (distance < detectionRadius) {
        follow = true;
    } else {
        follow = false;
    }

    // Move enemy only when following
    if (follow) {
        enemyTransform.velocity.x = directionX * enemyTransform.speed;
        enemyTransform.velocity.y = directionY * enemyTransform.speed;
    } else {
        enemyTransform.velocity.x = 0; // Stop moving if out of range
        enemyTransform.velocity.y = 0;
    }
}

static void petMovement(Entity& pet, Entity& player) {
    auto& petTransform = pet.getComponent<TransformComponent>();
    auto& playerTransform = player.getComponent<TransformComponent>();

    // Calculate the direction vector
    float directionX = playerTransform.position.x - petTransform.position.x;
    float directionY = playerTransform.position.y - petTransform.position.y;

    // Normalize the direction vector
    float length = std::sqrt(directionX * directionX + directionY * directionY);
    if (length > 0) {
        directionX /= length;
        directionY /= length;
    }

    // Smoothly interpolate the velocity
    float lerpFactor = 0.07f;
    float targetVelocityX = directionX * petTransform.speed;
    float targetVelocityY = directionY * petTransform.speed;

    if (Collision::AABB(player.getComponent<ColiderComponent>(), pet.getComponent<ColiderComponent>())) {
        follow = true;
    }

    if (follow)
    {
        petTransform.velocity.x += (targetVelocityX - petTransform.velocity.x) * lerpFactor;
        petTransform.velocity.y += (targetVelocityY - petTransform.velocity.y) * lerpFactor;
    }
}