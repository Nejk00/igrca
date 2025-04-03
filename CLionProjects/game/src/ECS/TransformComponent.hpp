
#include"../Vector2D.hpp"

#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

class TransformComponent : public Component {
    public:
      Vector2D position;
      Vector2D velocity;

      float speed = 0;
      int height = 32;
      int width = 32;
      int scale = 1;



    TransformComponent() {
        position.Zero();
    }
    TransformComponent(int sc) {
        position.x = 320;
        position.y = 320;
        scale=sc;
    }

    TransformComponent(float x, float y) {
        position.x = x;
        position.y = y;
    }
    TransformComponent(float x, float y, int h, int w, int sc, int sp) {
        position.x = x;
        position.y = y;
        scale = sc;
        width = w;
        height = h;
        speed = sp;
    }
    TransformComponent(float x, float y, int h, int w, int sc) {
        position.x = x;
        position.y = y;
        scale = sc;
        width = w;
        height = h;

    }

    void init() override {
        velocity.Zero();
    }
    void update() override {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed; // ce ne skrolamo mape je to movement

    }

};
#endif //POSITIONCOMPONENT_HPP
