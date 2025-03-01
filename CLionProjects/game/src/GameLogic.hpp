#include"ECS/ColiderComponent.hpp"

#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

static bool follow = false;

class GameLogic {
    public:
    static void enemyMovement(Entity&, Entity&);
    static void petMovement(Entity&, Entity&);
};



#endif //GAMELOGIC_HPP
