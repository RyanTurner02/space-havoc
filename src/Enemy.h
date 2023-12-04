#include "raylib.h"

class Enemy
{
public:
    Enemy(Vector3 position);
    Model getModel();
    Texture2D getTexture();
    BoundingBox getBoundingBox();
    Vector3 getPosition();
    void setPosition(Vector3 position);
    float getScale();
    int getType();
    void move();
    void destroy();

private:
    Model model;
    Texture2D texture;
    BoundingBox boundingBox;
    Vector3 position;
    float scale;
    int type;
};