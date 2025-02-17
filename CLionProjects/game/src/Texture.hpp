

#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include"game.hpp"

class Texture {
public:
    static SDL_Texture* LoadTexture(const char* filename);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);// ta str
};
#endif //TEXTURE_HPP
