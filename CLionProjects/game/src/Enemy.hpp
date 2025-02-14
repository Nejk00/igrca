
#ifndef ENEMY_HPP
#define ENEMY_HPP
#include"GameObject.hpp"
#include"Player.hpp"


class Enemy : public GameObject{
public:
    void update(){}
    void update(Player& player);
    void chase(const Player& player);
    Enemy (const char *texturesheet, int x, int y):GameObject(texturesheet, x, y) {};
};

#endif //ENEMY_HPP
