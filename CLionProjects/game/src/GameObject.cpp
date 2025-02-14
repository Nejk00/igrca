#include"GameObject.hpp"
#include"Texture.hpp"

GameObject::GameObject(const char *texturesheet, int x, int y) {
    objTexture = Texture :: LoadTexture(texturesheet);
    srcRect.h = 32;
    srcRect.w = 32;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = x;
    destRect.y = y;
    destRect.w = srcRect.w * 2;
    destRect.h = srcRect.h * 2;
}

void GameObject::update() {

}
void GameObject::render() {
    SDL_RenderCopy(Game :: renderer, objTexture, NULL, &destRect);
}



