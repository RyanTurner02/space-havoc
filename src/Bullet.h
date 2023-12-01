#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector3 position);
    void move();
    void draw();
    void destroy();

private:
    Vector3 position;
};