#include"Collision.hpp"
#include"ECS/ColiderComponent.hpp"


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




