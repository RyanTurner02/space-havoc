#include "raylib.h"

class Enemy
{
public:
    Enemy();
    Model getModel();
    Texture2D getTexture();
    BoundingBox getBoundingBox();
    Vector3 getPosition();
    float getScale();
    void move();
    void destroy();

private:
    Model model;
    Texture2D texture;
    BoundingBox boundingBox;
    Vector3 position;
    float scale;
};