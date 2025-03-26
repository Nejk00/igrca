

#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <SDL2/SDL.h>
struct Clock {
    static unsigned int last_tick_time;
    static unsigned int delta;

    static void tick() {
        unsigned int tick_time = SDL_GetTicks();
        delta = tick_time - last_tick_time;
        last_tick_time = tick_time;
    }
};

#endif //CLOCK_HPP
