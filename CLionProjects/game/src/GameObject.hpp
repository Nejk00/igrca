
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include <SDL_rect.h>
#include <SDL_render.h>

class GameObject {
    public:

    float xpos;
    float ypos;
    SDL_Rect srcRect, destRect;

    bool left=false;
    bool right=false;
    bool up=false;
    bool down=false;

    GameObject(const char* texturesheet, int x, int y);
    ~GameObject();
    virtual void update()=0;
    void render();


    private:

    SDL_Texture* objTexture;

};

#endif //GAMEOBJECT_HPP
