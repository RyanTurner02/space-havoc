#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector3 position);
    void move();
    void draw();
    Vector3 getPosition();
    float getRadius();

private:
    Vector3 position;
    float radius;
};