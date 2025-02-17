

#ifndef COLLISION_HPP
#define COLLISION_HPP
#include <SDL_rect.h>

#include "ECS/ColiderComponent.hpp"

class Collision;

class Collision {
    public:
    static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
    static bool AABB(const ColiderComponent& colA, const ColiderComponent& colB);
    static bool checkHorizontalCollision(const SDL_Rect& a, const SDL_Rect& b, float nextX);
    static bool checkVerticalCollision(const SDL_Rect& a, const SDL_Rect& b, float nextY);
};

#endif //COLLISION_HPP
