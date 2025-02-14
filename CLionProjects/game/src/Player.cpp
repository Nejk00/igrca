#include"Player.hpp"
#include <complex>
#include<SDL2/SDL_keyboard.h>



void Player::update(){
    HandleInput();
    destRect.x=xpos;
    destRect.y=ypos;


}
void Player::HandleInput() {
    const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);
    left=KeyboardState[SDL_SCANCODE_A];
    right=KeyboardState[SDL_SCANCODE_D];
    up=KeyboardState[SDL_SCANCODE_W];
    down=KeyboardState[SDL_SCANCODE_S];
    if(left){
        xpos-=5;
    }
    if(right){
        xpos+=5;
    }
    if(up){
        ypos-=5;
    }
    if(down){
        ypos+=5;
    }
    if(xpos<0)
        xpos=0;
    if(ypos<0)
        ypos=0;
    if(xpos>640-destRect.w)
        xpos=640-destRect.w;
    if(ypos>640-destRect.h)
        ypos=640-destRect.h;

}

