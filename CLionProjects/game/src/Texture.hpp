

#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include"game.hpp"

class Texture {
public:
    SDL_Texture *bulletTexture;
    SDL_Texture *hpTexture;

    static SDL_Texture* LoadTexture(const char* filename);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    static void RenderNumber(int number, int x, int y, float scale);
    static void RenderText(const std::string& text, int x, int y, float scale);
};
#endif //TEXTURE_HPP
