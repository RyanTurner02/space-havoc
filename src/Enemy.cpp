#include "Enemy.h"
#include "raylib.h"
#include "raymath.h"

#define SPEED 7.5

Enemy::Enemy(Vector3 position)
{
    model = LoadModel("objects/enemy/Alien1/Alien1.obj");
    texture = LoadTexture("objects/enemy/Alien1/Alien1.png");
    SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
    this->position = position;
    boundingBox = GetMeshBoundingBox(model.meshes[0]);
    scale = 0.25f;

    // scale the bounding box
    boundingBox.min = Vector3Scale(boundingBox.min, scale);
    boundingBox.max = Vector3Scale(boundingBox.max, scale);

    // rotate the bounding box
    float theta = PI;
    boundingBox.min = Vector3({boundingBox.min.x * cos(theta) + boundingBox.min.z * sin(theta), boundingBox.min.y, boundingBox.min.x * sin(theta) + boundingBox.min.z * cos(theta)});
    boundingBox.max = Vector3({boundingBox.max.x * cos(theta) + boundingBox.max.z * sin(theta), boundingBox.max.y, boundingBox.max.x * sin(theta) + boundingBox.max.z * cos(theta)});

    // move the bounding box
    boundingBox.min = Vector3Add((Vector3) {-boundingBox.min.x - 0.5f, boundingBox.min.y, boundingBox.min.z - 1.0f}, this->position);
    boundingBox.max = Vector3Add((Vector3) {-boundingBox.max.x - 0.5f, boundingBox.max.y, boundingBox.max.z - 1.0f}, this->position);
}

Model Enemy::getModel()
{
    return model;
}

Texture2D Enemy::getTexture()
{
    return texture;
}

BoundingBox Enemy::getBoundingBox()
{
    return boundingBox;
}

Vector3 Enemy::getPosition()
{
    return position;
}

void Enemy::setPosition(Vector3 position) {
    this->position = position;
}

float Enemy::getScale()
{
    return scale;
}

void Enemy::move()
{
    float deltaZ = SPEED * GetFrameTime();

    position.z -= deltaZ;
    boundingBox.min = Vector3Add(boundingBox.min, (Vector3){0.0f, 0.0f, -deltaZ});
    boundingBox.max = Vector3Add(boundingBox.max, (Vector3){0.0f, 0.0f, -deltaZ});
}

void Enemy::destroy()
{
    UnloadTexture(texture);
    UnloadModel(model);
}