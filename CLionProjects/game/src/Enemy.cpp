#include"Enemy.hpp"
#include"Player.hpp"



void Enemy :: update(Player& player) {
    chase(player);
}
void Enemy :: chase(const Player& player) {
    int speed=1;
    int deltaX=abs(player.destRect.x - destRect.x);
    int deltaY=abs(player.destRect.y - destRect.y);

    if (deltaX < 200 && deltaY < 200 && sqrt(deltaX * deltaY) < 200/1.41) { //Chasing
        if (player.destRect.x > destRect.x) {
            xpos+=speed;
        } else if (player.destRect.x < destRect.x) {
            xpos-=speed;
        }

        if (player.destRect.y > destRect.y) {
            ypos+=speed;
        } else if (player.destRect.y < destRect.y) {
            ypos-=speed;
        }
    }
    else { //Idle

    }

        if (xpos < 0) xpos = 0;
        if (ypos < 0) ypos = 0;
        if (xpos > 640 - destRect.w) xpos = 640 - destRect.w;
        if (ypos > 640 - destRect.h) ypos = 640 - destRect.h;

        destRect.x = xpos;
        destRect.y = ypos;
    }

