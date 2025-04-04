#include"Collision.hpp"



bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB) {
    if (recA.x +recA.w > recB.x &&
        recB.x +recB.w > recA.x &&
        recA.y +recA.h > recB.y &&
        recB.y +recB.h > recA.y) {
        return true;
    }
    return false;
}

bool Collision::AABB(const ColiderComponent &colA, const ColiderComponent &colB) {
    if (AABB(colA.collider, colB.collider)) {
        std::cout<<colA.tag<<" hit  "<<colB.tag<<std::endl;
        return true;
    }
    else {
        return false;
    }

}

void Collision::CheckCollisions(Entity& object, std::vector<Entity*>& entities) {
        if (!object.isActive() || !object.hasComponent<TransformComponent>() || !object.hasComponent<ColiderComponent>()) {
            return;
        }
        auto& objectTransform = object.getComponent<TransformComponent>();
        auto& objectCollider = object.getComponent<ColiderComponent>().collider;

        // Predict the object's next position
        float nextX = objectTransform.position.x + objectTransform.velocity.x * objectTransform.speed;
        float nextY = objectTransform.position.y + objectTransform.velocity.y * objectTransform.speed;


        SDL_Rect nextObjectCollider = objectCollider;
        nextObjectCollider.x = static_cast<int>(nextX);
        nextObjectCollider.y = static_cast<int>(nextY);

        // Check collision with all entities
        for (auto& entity : entities) {
            if (!entity || !entity->hasComponent<ColiderComponent>()) {
                continue; // Skip entities without a collider
            }

            auto& otherColliderComponent = entity->getComponent<ColiderComponent>();
            auto& otherCollider = otherColliderComponent.collider;

            // Skip checking collision with itself
            if (object.hasGroup(groupPlayers)) {
                if (entity->hasGroup(groupEnemies) || entity->hasGroup(groupPet) || entity->getComponent<ColiderComponent>().tag == "lab") {
                    continue;
                }
            }
            if (object.getComponent<TransformComponent>().position == entity->getComponent<TransformComponent>().position) {
                continue;
            }

            // Check if the predicted collider intersects with another collider
            if (SDL_HasIntersection(&nextObjectCollider, &otherCollider)) {
                // Separate horizontal and vertical checks
                SDL_Rect horizontalTest = objectCollider;
                horizontalTest.x = static_cast<int>(nextX);

                SDL_Rect verticalTest = objectCollider;
                verticalTest.y = static_cast<int>(nextY);

                // Resolve horizontal collision
                if (SDL_HasIntersection(&horizontalTest, &otherCollider)) {
                    objectTransform.velocity.x = 0; // Stop horizontal movement
                }

                // Resolve vertical collision
                if (SDL_HasIntersection(&verticalTest, &otherCollider)) {
                    objectTransform.velocity.y = 0; // Stop vertical movement
                }
            }
        }
}

bool Collision :: blockVision(const SDL_Rect &enemy, const SDL_Rect &player, std::vector<Entity*>& obstacles) {
    SDL_Rect Box1;

    // Create a bounding box that contains both the enemy and player
    Box1.x = std::min(enemy.x, player.x);
    Box1.y = std::min(enemy.y, player.y);
    Box1.w = std::abs(enemy.x - player.x);
    Box1.h = std::abs(enemy.y - player.y);

    // Check if any obstacle is inside this box
    for (auto& obstacle : obstacles) {
        if (obstacle->hasComponent<ColiderComponent>()) {
            if (SDL_HasIntersection(&Box1, &obstacle->getComponent<ColiderComponent>().collider)) {
                return true; // Vision blocked
            }
        }
    }
    return false; // No obstacles, enemy can see player
}
void Collision :: applyKnockback(Entity& a, Entity& b, float force) {
    auto& transA = a.getComponent<TransformComponent>();
    auto& transB = b.getComponent<TransformComponent>();

    // Vector from A to B
    float dx = transB.position.x - transA.position.x;
    float dy = transB.position.y - transA.position.y;

    float magnitude = std::sqrt(dx * dx + dy * dy);
    if (magnitude == 0) magnitude = 0.001f; // avoid division by zero

    // Normalize direction
    dx /= magnitude;
    dy /= magnitude;

    // Apply force in opposite directions
    transA.velocity.x = -dx * force;
    transA.velocity.y = -dy * force;

    transB.velocity.x = dx * force;
    transB.velocity.y = dy * force;

    // Optional flags
    transA.isKnockbacked = true;
    transB.isKnockbacked = true;

    // Optional timer setup (if you're using one)
    transA.knockbackTime = 500;
    transB.knockbackTime = 500;
}





