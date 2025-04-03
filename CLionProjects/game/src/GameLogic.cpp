#include"GameLogic.hpp"
#include <vector>
#include"Collision.hpp"
#include"Map.hpp"


void GameLogic :: CameraSystem(Entity& player) {
    Game::camera.x = player.getComponent<TransformComponent>().position.x - Game :: SCREEN_WIDTH/2;
    Game::camera.y = player.getComponent<TransformComponent>().position.y - Game :: SCREEN_HEIGHT/2;

    if (Game::camera.x < 0) Game::camera.x = 0;
    if (Game::camera.y < 0) Game::camera.y = 0;
    if (Game::camera.x + Game::SCREEN_WIDTH > Map::sizeX * 32) Game::camera.x = Map::sizeX * 32 - Game :: SCREEN_WIDTH;
    if (Game::camera.y + Game::SCREEN_HEIGHT > Map::sizeY * 32) Game::camera.y = Map::sizeY * 32 - Game :: SCREEN_HEIGHT;
}
