#include "raylib.h"

class Player
{
public:
    Player();
    Model getModel();
    Texture2D getTexture();
    BoundingBox getBoundingBox();
    Vector3 getPosition();
    Vector3 getStartingPosition();
    void setPosition(Vector3 position);
    void moveLeft();
    void moveRight();
    void destroy();

private:
    Model model;
    Texture2D texture;
    BoundingBox boundingBox;
    Vector3 position;
};