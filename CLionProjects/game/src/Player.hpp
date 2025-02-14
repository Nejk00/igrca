
#include"GameObject.hpp"
#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player : public GameObject{
  public:
    void update();
    void HandleInput();
    Player (const char *texturesheet, int x, int y):GameObject(texturesheet, x, y) {};
};

#endif //PLAYER_HPP
