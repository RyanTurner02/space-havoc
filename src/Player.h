#include "raylib.h"

class Player
{
public:
    Player();
    Model getModel();
    Texture2D getTexture();
    Vector3 getPosition();
    void moveLeft();
    void moveRight();
    void shoot();
    void destroy();

private:
    Model model;
    Texture2D texture;
    Vector3 position;
};