
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

struct Animation {
    int index;
    int frames;
    int speed;

    Animation(){}
    Animation(int i, int f, int s) {
        index = i;
        frames = f;
        speed = s;
    }
};

#endif //ANIMATION_HPP
