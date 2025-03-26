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

void Collision::CheckCollisions(Entity& object,std::vector<Entity*>& entities) {
    // Get the object's transform and collider
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
            if (entity->hasGroup(groupEnemies) || entity->hasGroup(groupPet) || entity->hasGroup(groupPlayers)) {
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

/*void Collision::CheckCollisions(Entity& object, std::vector<Entity*>& tiles) {
    // Get the object's transform and collider
    auto& objectTransform = object.getComponent<TransformComponent>();
    auto& objectCollider = object.getComponent<ColiderComponent>().collider;

    // Predict the object's next position
    float nextX = objectTransform.position.x + objectTransform.velocity.x * objectTransform.speed;
    float nextY = objectTransform.position.y + objectTransform.velocity.y * objectTransform.speed;

    SDL_Rect nextObjectCollider = objectCollider;
    nextObjectCollider.x = static_cast<int>(nextX);
    nextObjectCollider.y = static_cast<int>(nextY);

    // Check collision with all colliders
    for (auto& otherColliderComponent : tiles) {

        auto& otherCollider = otherColliderComponent->getComponent<ColiderComponent>().collider;

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
}*/




