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
    bool getIsSprinting();
    void setIsSprinting(bool isSprinting);
    void moveLeft();
    void moveRight();
    void destroy();

private:
    float getDeltaX();
    Model model;
    Texture2D texture;
    BoundingBox boundingBox;
    Vector3 position;
    bool isSprinting;
};