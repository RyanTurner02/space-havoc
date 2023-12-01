#include "Bullet.h"
#include "Player.h"
#include <iostream>

#define BULLET_SPEED 25

Bullet::Bullet(Vector3 position)
{
    this->position = position;
    this->position.x += 1.7f;
    this->position.y += 0.75f;
    this->position.z += 0.25f;
}

void Bullet::draw()
{
    DrawSphere(position, 0.25f, RED);
}

void Bullet::move()
{
    float deltaZ = BULLET_SPEED * GetFrameTime();
    position.z += deltaZ;
}