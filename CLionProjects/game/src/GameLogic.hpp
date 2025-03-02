
#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include"ECS/ColiderComponent.hpp"
#include<vector>

static bool follow = false;

class GameLogic {
    public:
    static void enemyMovement(Entity&, std::vector<Entity*>&);
    static void petMovement(Entity&, std::vector<Entity*>&);
    static void CameraSystem(Entity&);
};



#endif //GAMELOGIC_HPP
