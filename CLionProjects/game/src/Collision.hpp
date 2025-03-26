

#ifndef COLLISION_HPP
#define COLLISION_HPP
#include <SDL_rect.h>
#include "ECS/Components.hpp"

enum groupLables : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupPet,
    groupProjectile,
};

class Collision {
    public:
    static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
    static bool AABB(const ColiderComponent& colA, const ColiderComponent& colB);
    static void CheckCollisions(Entity&, Entity&, std::vector<ColiderComponent*>&);
    static void CheckCollisions(Entity&, std::vector<Entity*>&);
    /*static void CheckCollisions(Entity&, std::vector<Entity*>&);*/
};


#endif //COLLISION_HPP
